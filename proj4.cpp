#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sstream>


#define MAX_FRAMES 20


int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <Policy> <Frames> <Reference String>" << endl;
        return 1;
    }

    string policy = argv[1];
    if (policy != "lru" && policy != "fifo" && policy != "min") {
        cerr << "Error: Invalid policy. Use 'lru', 'fifo', or 'min'." << endl;
        return 1;
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

    /**/
    for (int i = 0; i < numPageRefs; i++) {
        cout << pageRefs[i] << " ";
    }
    cout << endl;
    /**/
    // Implement the page replacement algorithm here

    return 0;
}