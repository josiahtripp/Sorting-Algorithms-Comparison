#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>
#include <sstream>

#define UNSORTED_DATA_FILENAME "UnsortedData.txt"
#define SORTED_DATA_FILENAME_SUFFIX "_SortedData.txt"
#define UNSORTED_DATA_AMOUNT 10000
#define UNSORTED_DATA_MINIMUM 0
#define UNSORTED_DATA_MAXIMUM 1000

#define RANDINT(a, b) (rand() % (b - a + 1)) + a

/* Sorting Algorithm Implementations */
std::vector<int> CountingSort(const std::vector<int>& Data);
std::vector<int> MergeSort(const std::vector<int>& Data);
std::vector<int> QuickSort(const std::vector<int>& Data);
std::vector<int> BubbleSort(const std::vector<int>& Data);
std::vector<int> InsertionSort(const std::vector<int>& Data);

int GenerateUnsortedData();

bool FileExists(std::string Filename);

void PrintSortOptions();

bool LoadUnsortedData(std::vector<int>& Data);

bool SaveSortedData(std::string Filename, const std::vector<int>& Data);

bool CheckSortAndIntegrity(const std::vector<int>& Data);

void PrintElapsedTime(std::chrono::nanoseconds ElapsedTime);

int GetUserSelection();

void PrintSortingReport(std::vector<std::chrono::nanoseconds> ElapsedTimes);

std::string FormatNumber(int Input);

std::string FormatTime(std::chrono::nanoseconds Time);

std::chrono::nanoseconds SortData(const std::vector<int>& Data, std::vector<int>& NewData, int AlgorithmIndex);

std::string AlgorithmName[] = {

    "counting sort",
    "merge sort",
    "quick sort",
    "bubble sort",
    "insertion sort"
};

std::vector<int> (*AlgorithmFunction[]) (const std::vector<int>& Data) = {

    CountingSort,
    MergeSort,
    QuickSort,
    BubbleSort,
    InsertionSort
};

int main(){

    std::vector<int> Data;
    srand(time(0)); // Set seed for random data generation

    /* Generate unsorted data file if needed */
    if(GenerateUnsortedData() == -1){
        return 1;
    }

    /* Load in unsorted data from file */
    if(!LoadUnsortedData(Data)){
        return 1;
    }

    /* Store data for integrity check later */
    CheckSortAndIntegrity(Data);

    /* Get user selection of menu option */
    int Selection = GetUserSelection();

    if(Selection == 5){// Run all sorting algorithms
        
        std::vector<std::chrono::nanoseconds> ElapsedTimes;
        
        for(int i = 0; i < 5; i++){
            
            std::vector<int> NewData;
            ElapsedTimes.push_back(SortData(Data, NewData, i));
            std::cout << std::endl;
            if(!CheckSortAndIntegrity(NewData)){
                return 1;
            }
            if(!SaveSortedData(AlgorithmName[i] + SORTED_DATA_FILENAME_SUFFIX, NewData)){
                return 1;
            }
            std::cout << "__________________________________________________" << std::endl << std::endl;
        }

        PrintSortingReport(ElapsedTimes);
    }
    else{// Run the selected sorting algorithm

        std::vector<int> NewData;
        PrintElapsedTime(SortData(Data, NewData, Selection));
        if(!CheckSortAndIntegrity(NewData)){
            return 1;
        }   
        if(!SaveSortedData(AlgorithmName[Selection] + SORTED_DATA_FILENAME_SUFFIX, NewData)){
            return 1;
        }
    }
    return 0;
}

std::vector<int> CountingSort(const std::vector<int>& Data){

    int MinimumDataValue = *min_element(Data.begin(), Data.end());
    int MaximumDataValue = *max_element(Data.begin(), Data.end());

    std::vector<int> CountArray(MaximumDataValue - MinimumDataValue + 1);
    for(int i = 0; i < CountArray.size(); i++){
        CountArray[i] = 0;
    }// Set to zero

    for(int i = 0; i < Data.size(); i++){
        CountArray[Data[i] - MinimumDataValue]++;
    }// Count the elements

    for(int i = 1; i < CountArray.size(); i++){
        CountArray[i] = CountArray[i-1] + CountArray[i];
    }// Update count array with sums

    std::vector<int> OutputData(Data.size());
    for(int i = Data.size() - 1; i > -1; i--){
        OutputData[--CountArray[Data[i]]] = Data[i];
    }
    return OutputData;
}

std::vector<int> MergeSort(const std::vector<int>& Data){

    if(Data.size() <= 1){
        return Data;
    }// List contains one entity, is already sorted

    std::vector<int> RightData, LeftData, NewData;

    for(int i = 0; i < Data.size() / 2; i++){
        RightData.push_back(Data[i]);
    }// Copy right half of data

    for(int i = Data.size() / 2; i < Data.size(); i++){
        LeftData.push_back(Data[i]);
    }// Copy Left half of data

    RightData = MergeSort(RightData);
    LeftData = MergeSort(LeftData);

    int RIndex = 0, LIndex = 0;
    while(RIndex < RightData.size() && LIndex < LeftData.size()){
        if(RightData[RIndex] < LeftData[LIndex]){
            NewData.push_back(RightData[RIndex]);
            RIndex++;
        }
        else{
            NewData.push_back(LeftData[LIndex]);
            LIndex++;
        }
    }

    while(RIndex < RightData.size()){
        NewData.push_back(RightData[RIndex]);
        RIndex++;
    }

    while(LIndex < LeftData.size()){
        NewData.push_back(LeftData[LIndex]);
        LIndex++;
    }
    return NewData;
}

std::vector<int> QuickSort(const std::vector<int>& Data){

    if(Data.size() <= 1){
        return Data;
    }// List contains one entity, is already sorted

    std::vector<int> RightData, LeftData, NewData;

    // Choose a random value as the pivot
    int PivotIndex = RANDINT(0, Data.size() - 1);
    int PivotValue = Data[PivotIndex];

    // Values smaller than P go left, greater or equal go right (skip the actual pivot value)
    for(int i = 0; i < Data.size(); i++){

        if(Data[i] < PivotValue){
            LeftData.push_back(Data[i]);
        }
        else if(i == PivotIndex){
        }
        else{
            RightData.push_back(Data[i]);
        }
    }

    // Sort parititions
    LeftData = QuickSort(LeftData);
    RightData = QuickSort(RightData);
    
    // Stitch back together
    for(int Element : LeftData){
        NewData.push_back(Element);
    }
    NewData.push_back(PivotValue);
    for(int Element : RightData){
        NewData.push_back(Element);
    }
    return NewData;
}

std::vector<int> BubbleSort(const std::vector<int>& Data){

    std::vector<int> NewData = Data;
    bool swapped;

    for(int i = 0; i < NewData.size() - 1; i++){

        swapped = false;
        for(int j = 0; j < NewData.size() - i - 1; j++){

            if(NewData[j] > NewData[j + 1]){
                int tmp = NewData[j];
                NewData[j] = NewData[j + 1];
                NewData[j + 1] = tmp;
                swapped = true;
            }
        }

        if(!swapped){
            return NewData;
        }
    }
    return NewData;
}

std::vector<int> InsertionSort(const std::vector<int>& Data){

    std::vector<int> NewData = Data;
    for(int i = 1; i < NewData.size(); i++){
        
        int Selected = NewData[i];
        int j = i - 1;
        while(j >= 0 && NewData[j] > Selected){
            NewData[j + 1] = NewData[j];
            j--;
        }
        NewData[j + 1] = Selected;
    }
    return NewData;
}

int GenerateUnsortedData(){

    if(FileExists(UNSORTED_DATA_FILENAME)){
        return 0;
    }

    std::cout << "No unsorted data file exists. Generating new data to \"";
    std::cout << UNSORTED_DATA_FILENAME << "\"..." << std::endl;

    std::ofstream OutputStream;
    OutputStream.open(UNSORTED_DATA_FILENAME);

    if(!OutputStream){
        std::cerr << "File \"" << UNSORTED_DATA_FILENAME << "\" could not be created / opened for writing." << std::endl;
        std::cout << "Unsorted data generation failed." << std::endl << std::endl;
        OutputStream.close();
        return -1;
    }

    for(int i = 0; i < UNSORTED_DATA_AMOUNT; i++){

        OutputStream << RANDINT(UNSORTED_DATA_MINIMUM, UNSORTED_DATA_MAXIMUM) << ' ';
    }

    if(!OutputStream){
        std::cerr << "Data could not be written to \"" << UNSORTED_DATA_FILENAME << "\"." << std::endl;
        std::cout << "Unsorted data generation failed." << std::endl << std::endl;
        OutputStream.close();
        return -1;
    }

    OutputStream.close();
    std::cout << "Unsorted data generated successfully." << std::endl << std::endl;
    return UNSORTED_DATA_AMOUNT;
}

bool FileExists(std::string Filename){

    std::ifstream TestInputStream;
    TestInputStream.open(Filename);

    if(TestInputStream.is_open()){

        TestInputStream.close();
        return true;
    }
    return false;
}

void PrintSortOptions(){

    std::cout << R"(Please Select a Sorting Algorithm to Use: (Enter a number)
    1. Counting Sort
    2. Merge Sort
    3. Quick Sort
    4. Bubble Sort
    5. Insertion Sort
    6. [Test All Algorithms])" << std::endl << std::endl;
}

bool LoadUnsortedData(std::vector<int>& Data){

    std::cout << "Loading unsorted data from \"" << UNSORTED_DATA_FILENAME << "\"..." << std::endl;

    std::ifstream InputStream;
    InputStream.open(UNSORTED_DATA_FILENAME);

    std::vector<int> NewData;

    if(!InputStream){
        std::cerr << "File \"" << UNSORTED_DATA_FILENAME << "\" could not be opened for reading." << std::endl;
        std::cout << "Unsorted data could not be loaded." << std::endl << std::endl;
        return false;
    }

    int TmpDataEntity;
    InputStream >> TmpDataEntity;
    
    while(InputStream){

        NewData.push_back(TmpDataEntity);
        InputStream >> TmpDataEntity;
    }

    Data = NewData;
    InputStream.close();
    std::cout << "Loaded unsorted data successfully." << std::endl << std::endl;
    return true;
}

bool SaveSortedData(std::string Filename, const std::vector<int>& Data){

    std::cout << "Outputting Sorted Data to \"" << Filename << "\"..." << std::endl;

    std::ofstream OutputStream;
    OutputStream.open(Filename);

    if(!OutputStream){
        std::cerr << "Could Not Open / Create File \"" << Filename << "\" For Writing." << std::endl;
        OutputStream.close();
        return false;
    }

    for(int DataEntity : Data){
        OutputStream << DataEntity << ' ';
    }

    if(!OutputStream){
        std::cerr << "Could Not Write Data To File \"" << Filename << "\"." << std::endl;
        OutputStream.close();
        return false;
    }

    OutputStream.close();
    std::cout << "Sorted Data Outputted Successfully." << std::endl;
    return true;
}

bool CheckSortAndIntegrity(const std::vector<int>& Data){

    static std::vector<int> DataCounts;

    if(DataCounts.size() == 0){
        std::cout << "Tabulating data for post-sort integrity check..." << std::endl;
    }
    else{
        std::cout << "Checking Sorted Data..." << std::endl;
    }

    int MinimumDataValue = *min_element(Data.begin(), Data.end());
    int MaximumDataValue = *max_element(Data.begin(), Data.end());

    std::vector<int> CountArray(MaximumDataValue - MinimumDataValue + 1);
    for(int i = 0; i < CountArray.size(); i++){
        CountArray[i] = 0;
    }// Set to zero

    for(int i = 0; i < Data.size(); i++){
        CountArray[Data[i] - MinimumDataValue]++;
    }// Count the elements
    
    if(DataCounts.size() == 0){
        DataCounts = CountArray;
        std::cout << "Data tabulated successfully." << std::endl << std::endl;
        return true;
    }
    else{
        if(DataCounts.size() != CountArray.size()){
            
            std::cout << "Data Sort And Integrity Check Failed." << std::endl << std::endl;
            return false;
        }

        int lastInt = INT32_MIN;
        for(int i = 0; i < Data.size(); i++){

            if(Data[i] < lastInt){
                std::cerr << "Error In Data Sorting" << std::endl;
                std::cout << "Data Sort And Integrity Check Failed." << std::endl << std::endl;
                return false;
            }
            lastInt = Data[i];
        }

        for(int i = 0; i < DataCounts.size(); i++){
            if(DataCounts[i] != CountArray[i]){
                std::cerr << "Error In Data Integrity." << std::endl;
                std::cout << "Data Sort And Integrity Check Failed." << std::endl << std::endl;
                return false;
            }
        }

        std::cout << "Data Sort And Integrity Check Successfull." << std::endl << std::endl;
        return true;
    }
}

void PrintElapsedTime(std::chrono::nanoseconds ElapsedTime){

    auto MinutesElapsed = std::chrono::duration_cast<std::chrono::minutes>(ElapsedTime);
    auto SecondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(ElapsedTime);
    SecondsElapsed %= 60;
    auto MillisecondsElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(ElapsedTime);
    MillisecondsElapsed %= 1000;
    auto MicrosecondsElapsed = std::chrono::duration_cast<std::chrono::microseconds>(ElapsedTime);
    MicrosecondsElapsed %= 1000;
    auto NanosecondsElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(ElapsedTime);
    NanosecondsElapsed %= 1000;

    std::cout << "Time Elapsed:" << std::endl;
    if(MinutesElapsed.count() > 0){
        std::cout << '\t' << std::setw(3) << MinutesElapsed.count() << " Minutes" << std::endl;
    }
    if(SecondsElapsed.count() > 0){
        std::cout << '\t' << std::setw(3) << SecondsElapsed.count() << " Seconds" << std::endl;
    }
    if(MillisecondsElapsed.count() > 0){
        std::cout << '\t' << std::setw(3) << MillisecondsElapsed.count() << " Milliseconds" << std::endl;
    }
    if(MicrosecondsElapsed.count() > 0){
        std::cout << '\t' << std::setw(3) << MicrosecondsElapsed.count() << " Microseconds" << std::endl;
    }
    if(NanosecondsElapsed.count() > 0){
        std::cout << '\t' << std::setw(3) << NanosecondsElapsed.count() << " Nanoseconds";   
    }
    std::cout << std::endl << std::endl;
}

int GetUserSelection(){

    char Selection = '\0';
    while(Selection < '1' || Selection > '6'){

        // Print an error message in response to invalid input
        if(Selection != '\0'){
            std::cout << "'" << Selection << "' is not a valid option." << std::endl << std::endl;
        }

        // Print menu and get user input
        PrintSortOptions();
        std::cout << "Selection: ";
        std::cin >> Selection;
        
        // Clear the buffer from any excess characters
        while(getchar() != '\n'){}
    }
    std::cout << std::endl;
    return int(Selection) - 49;
}

std::chrono::nanoseconds SortData(const std::vector<int>& Data, std::vector<int>& NewData, int AlgorithmIndex){

    std::cout << "Sorting Data With The " << AlgorithmName[AlgorithmIndex] << " Algorithm..." << std::endl;
    auto StartTime = std::chrono::high_resolution_clock::now();
    NewData = AlgorithmFunction[AlgorithmIndex](Data);
    auto ElapsedTime = std::chrono::high_resolution_clock::now() - StartTime;
    std::cout << "Data sort completed" << std::endl;
    return ElapsedTime;
}

void PrintSortingReport(std::vector<std::chrono::nanoseconds> ElapsedTimes){

    std::string TB1 = "==================================================="; // Top-Bar 1
    std::string TB2 = "---------------------------------------------------"; // Top-bar 2

    std::cout << TB1 << std::endl << "|                 Sorting Results                 |" << std::endl;
    std::cout << TB1 << std::endl << std::endl;

    std::cout << TB2 << std::endl << "| Elements Sorted:       | ";
    std::cout << std::right << std::setw(24);
    std::cout << FormatNumber(UNSORTED_DATA_AMOUNT) << " |" << std::endl;
    std::cout << TB2 << std::endl << "| Element Range Minimum: | ";
    std::cout << std::setw(24) << FormatNumber(UNSORTED_DATA_MINIMUM);
    std::cout << " |" << std::endl << TB2 << std::endl;
    std::cout << "| Element Range Maximum: | " << std::setw(24);
    std::cout << FormatNumber(UNSORTED_DATA_MAXIMUM) << " |" << std::endl;
    std::cout << TB2 << std::endl;

    std::cout << TB2 << std::endl << "|      Algorithm:     |        Time Elapsed:      |";
    std::cout << std::endl << TB2 << std::endl;

    for(int i = 0; i < 5; i++){
        
        std::cout << "| " << std::left << std::setw(19) << AlgorithmName[i];
        std::cout << " | " << std::right << std::setw(25) << FormatTime(ElapsedTimes[i]);
        std::cout << " |" << std::endl << TB2 << std::endl;
    }
}

std::string FormatNumber(int Input){
    
    std::stringstream ss;
    ss << Input;

    std::string NumberString = ss.str();
    std::string TMPString, OutputString;

    for(int i = NumberString.length(); i > -1; i--){

        TMPString.push_back(NumberString[i]);
        if(Input >= 1000 && NumberString.length() - i == 3){
            TMPString.push_back(',');
        }
        if(Input >= 1000000 && NumberString.length() - i == 6){
            TMPString.push_back(',');
        }
        if(Input >= 1000000000 && NumberString.length() - i == 9){
            TMPString.push_back(',');
        }
    }

    for(int i = TMPString.length(); i > -1; i--){
        OutputString.push_back(TMPString[i]);
    }
    return OutputString;
}

std::string FormatTime(std::chrono::nanoseconds Time){

    auto MinutesElapsed = std::chrono::duration_cast<std::chrono::minutes>(Time);
    auto SecondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(Time);
    SecondsElapsed %= 60;
    auto MillisecondsElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Time);
    MillisecondsElapsed %= 1000;
    auto MicrosecondsElapsed = std::chrono::duration_cast<std::chrono::microseconds>(Time);
    MicrosecondsElapsed %= 1000;
    auto NanosecondsElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(Time);
    NanosecondsElapsed %= 1000;

    std::stringstream ss;

    if(MinutesElapsed.count() > 0){
        ss << MinutesElapsed.count() << "m ";
    }
    if(SecondsElapsed.count() > 0){
        ss << SecondsElapsed.count() << "s ";
    }
    if(MillisecondsElapsed.count() > 0){
        ss << MillisecondsElapsed.count() << "ms ";
    }
    if(MicrosecondsElapsed.count() > 0){
        ss << MicrosecondsElapsed.count() << "us ";
    }
    if(NanosecondsElapsed.count() > 0){
        ss << NanosecondsElapsed.count() << "ns ";
    }
    return ss.str();
}