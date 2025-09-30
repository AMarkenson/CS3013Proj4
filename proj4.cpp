#include <iostream>
using namespace std;
#include <fcntl.h>
#include <unistd.h>
#include <sstream>


#define MAX_FRAMES 20

int fifo(int pageRefs[], int numPageRefs, int frames, bool trace) {
    int pageFaults = 0;
    int frameList[frames];
    int nextToReplace = 0;
    for (int i = 0; i < frames; i++) {
        frameList[i] = -1;
    }
    for (int i = 0; i < numPageRefs; i++) {
        bool hit = false;
        for (int j = 0; j < frames; j++) {
            if (frameList[j] == pageRefs[i]) {
                hit = true;
                break;
            }
        }
        if (!hit) {
            frameList[nextToReplace] = pageRefs[i];
            nextToReplace = (nextToReplace + 1) % frames;
            pageFaults++;
        }
        if (trace) {
            cout << pageRefs[i] << " ";
            if (!hit) {
                cout << "P ";
            } else {
                cout << "- ";
            }
            cout << "Frames: ";
            for (int j = 0; j < frames; j++) {
                if (frameList[j] != -1) {
                    cout << frameList[j] << " ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

    //cout << "page faults: " << pageFaults << endl;
    return pageFaults;
}

int lru(int pageRefs[], int numPageRefs, int frames, bool trace) {
    int pageFaults = 0;
    int frameList[frames];
    int timeList[frames];
    int time = 0;
    for (int i = 0; i < frames; i++) {
        frameList[i] = -1;
        timeList[i] = 0;
    }
    for (int i = 0; i < numPageRefs; i++) {
        bool hit = false;
        for (int j = 0; j < frames; j++) {
            if (frameList[j] == pageRefs[i]) {
                hit = true;
                timeList[j] = time++;
                break;
            }
        }
        if (!hit) {
            int lruIndex = 0;
            for (int j = 1; j < frames; j++) {
                if (timeList[j] < timeList[lruIndex]) {
                    lruIndex = j;
                }
            }
            frameList[lruIndex] = pageRefs[i];
            timeList[lruIndex] = time++;
            pageFaults++;
        }
        if (trace) {
            cout << pageRefs[i] << " ";
            if (!hit) {
                cout << "P ";
            } else {
                cout << "- ";
            }
            cout << "Frames: ";
            for (int j = 0; j < frames; j++) {
                if (frameList[j] != -1) {
                    cout << frameList[j] << " ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }
    //cout << "page faults: " << pageFaults << endl;
    return pageFaults;
}

int optimal(int pageRefs[], int numPageRefs, int frames, bool trace) {
    int pageFaults = 0;
    int frameList[frames];
    for (int i = 0; i < frames; i++) {
        frameList[i] = -1;
    }
    for (int i = 0; i < numPageRefs; i++) {
        bool hit = false;
        for (int j = 0; j < frames; j++) {
            if (frameList[j] == pageRefs[i]) {
                hit = true;
                break;
            }
        }
        if (!hit) {
            int replaceIndex = -1;
            int farthest = i;
            for (int j = 0; j < frames; j++) {
                if (frameList[j] == -1) {
                    replaceIndex = j;
                    break;
                }
                int k;
                for (k = i + 1; k < numPageRefs; k++) {
                    if (frameList[j] == pageRefs[k]) {
                        if (k > farthest) {
                            farthest = k;
                            replaceIndex = j;
                        }
                        break;
                    }
                }
                if (k == numPageRefs) {
                    replaceIndex = j;
                    break;
                }
            }
            frameList[replaceIndex] = pageRefs[i];
            pageFaults++;
        }
        if (trace) {
            cout << pageRefs[i] << " ";
            if (!hit) {
                cout << "P ";
            } else {
                cout << "- ";
            }
            cout << "Frames: ";
            for (int j = 0; j < frames; j++) {
                if (frameList[j] != -1) {
                    cout << frameList[j] << " ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

    //cout << "page faults: " << pageFaults << endl;
    return pageFaults;
}

int clock(int pageRefs[], int numPageRefs, int frames, bool trace) {
    int pageFaults = 0;
    int frameList[frames];
    bool refBits[frames];
    int pointer = 0;
    for (int i = 0; i < frames; i++) {
        frameList[i] = -1;
        refBits[i] = false;
    }
    for (int i = 0; i < numPageRefs; i++) {
        bool hit = false;
        for (int j = 0; j < frames; j++) {
            if (frameList[j] == pageRefs[i]) {
                hit = true;
                refBits[j] = true;
                break;
            }
        }
        if (!hit) {
            while (true) {
                if (refBits[pointer] == false) {
                    frameList[pointer] = pageRefs[i];
                    refBits[pointer] = true;
                    pointer = (pointer + 1) % frames;
                    break;
                } else {
                    refBits[pointer] = false;
                    pointer = (pointer + 1) % frames;
                }
            }
            pageFaults++;
        }
        if (trace) {
            cout << pageRefs[i] << " ";
            if (!hit) {
                cout << "P ";
            } else {
                cout << "- ";
            }
            cout << "Frames: ";
            for (int j = 0; j < frames; j++) {
                if (frameList[j] != -1) {
                    cout << frameList[j] << " ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

    //cout << "page faults: " << pageFaults << endl;
    return pageFaults;
}

int lfu(int pageRefs[], int numPageRefs, int frames, bool trace) {
    int pageFaults = 0;
    int frameList[frames];
    int freqList[frames];
    for (int i = 0; i < frames; i++) {
        frameList[i] = -1;
        freqList[i] = 0;
    }
    for (int i = 0; i < numPageRefs; i++) {
        bool hit = false;
        for (int j = 0; j < frames; j++) {
            if (frameList[j] == pageRefs[i]) {
                hit = true;
                freqList[j]++;
                break;
            }
        }
        if (!hit) {
            int lfuIndex = 0;
            for (int j = 1; j < frames; j++) {
                if (freqList[j] < freqList[lfuIndex]) {
                    lfuIndex = j;
                }
            }
            frameList[lfuIndex] = pageRefs[i];
            freqList[lfuIndex] = 1;
            pageFaults++;
        }
        if (trace) {
            cout << pageRefs[i] << " ";
            if (!hit) {
                cout << "P ";
            } else {
                cout << "- ";
            }
            cout << "Frames: ";
            for (int j = 0; j < frames; j++) {
                if (frameList[j] != -1) {
                    cout << frameList[j] << " ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

    //cout << "page faults: " << pageFaults << endl;
    return pageFaults;
}




int main(int argc, char* argv[]) {
    bool trace = false;
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <Policy> <Frames> <Reference String> <Trace (Optional)>" << endl;
        return 1;
    }

    string policy = argv[1];
    if (policy != "lru" && policy != "fifo" && policy != "min" && policy != "clock" && policy != "lfu" && policy != "all") {
        cerr << "Error: Invalid policy. Use 'lru', 'fifo', 'min', 'clock', 'lfu', or 'all'." << endl;
        return 1;
    }
    if (policy == "all" && argc == 5) {
        cerr << "Error: Trace file should not be provided when policy is 'all'." << endl;
        return 1;
    }
    if (argc == 5 && argv[4] == string("trace")) {
        trace = true;
    }

    int frames = stoi(argv[2]);
    if (frames < 3) {
        cerr << "Error: Number of frames must be at least 3." << endl;
        return 1;
    } else {
        frames = min(frames, MAX_FRAMES);
    }

    string referenceString = argv[3];
    int pageRefs[1024];

    int fdIn = open(referenceString.c_str(), O_RDONLY);
    if (fdIn < 0) {
        cerr << "Error: Unable to open reference string file." << endl;
        return 1;
    }

    // Read the reference string from the file
    char buffer[1024];
    ssize_t bytesRead = read(fdIn, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        cerr << "Error: Unable to read reference string file." << endl;
        close(fdIn);
        return 1;
    }
    buffer[bytesRead] = '\0';

    // Parse the reference string into page references
    istringstream iss(buffer);
    string page;
    int index = 0;
    while (iss >> page && index < 1024) {
        pageRefs[index++] = stoi(page);
    }

    close(fdIn);
    int numPageRefs = index;
    int pageFaults = 0;

    /**/
    for (int i = 0; i < numPageRefs; i++) {
        cout << pageRefs[i] << " ";
    }
    cout << endl;
    /**/
    // Implement the page replacement algorithm here
    if (policy == "fifo") {
        pageFaults = fifo(pageRefs, numPageRefs, frames, trace);
    } else if (policy == "lru") {
        pageFaults = lru(pageRefs, numPageRefs, frames, trace);
    } else if (policy == "min") {
        pageFaults = optimal(pageRefs, numPageRefs, frames, trace);
    } else if (policy == "clock") {
        pageFaults = clock(pageRefs, numPageRefs, frames, trace);
    } else if (policy == "lfu") {
        pageFaults = lfu(pageRefs, numPageRefs, frames, trace);
    } else if (policy == "all") {
        // Implement
    }


    // Output the results
    cout << policy << " policy with " << frames << " frames: " << pageFaults << " page faults for " << numPageRefs << " references." << endl;

    return 0;
}