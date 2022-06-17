#include "fstream"
#include <iostream>
#include <thread>
#include <sstream>
#include "BoundedQueue.h"
#include "UnboundedQueue.h"

vector<BoundedQueue *> producerQueues;
vector<UnboundedQueue *> coEditorQueues(3);
BoundedQueue *screenQueue;

void producer(int id, int newsNum) {
    int i;
    for (i = 0; i < newsNum / 3; i += 1) {
        producerQueues[id]->insert("Producer " + std::to_string(id + 1) + " Sports " + std::to_string(i));
    }
    for (i = 0; i < newsNum / 3; i += 1) {
        producerQueues[id]->insert("Producer " + std::to_string(id + 1) + " News " + std::to_string(i));
    }
    for (i = 0; i < newsNum / 3; i += 1) {
        producerQueues[id]->insert("Producer " + std::to_string(id + 1) + " Weather " + std::to_string(i));
    }
    int j = 3 - (newsNum % 3);
    while (j < 3) {
        producerQueues[id]->insert("Producer " + std::to_string(id + 1) + " Weather " + std::to_string(i + j - 1));
        ++j;
    }
    producerQueues[id]->insert("DONE");
}

void dispatcher() {
    vector<BoundedQueue *> activeProducers = producerQueues;
    while (!activeProducers.empty()) {
        for (int i = 0; i < activeProducers.size(); i++) {
            std::string str = activeProducers[i]->remove();
            if (str == "DONE") {
                activeProducers.erase(activeProducers.begin() + i);
            } else {
                coEditorQueues[0]->insert(str);
                if (str.find("Sports") != std::string::npos) {
                    coEditorQueues[0]->insert(str);
                } else if (str.find("News") != std::string::npos) {
                    coEditorQueues[1]->insert(str);
                } else if (str.find("Weather") != std::string::npos) {
                    coEditorQueues[2]->insert(str);
                }
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        coEditorQueues[i]->insert("DONE");
    }
}

void coEditor(int queueId) {
    while (true) {
        std::string str = coEditorQueues[queueId]->remove();
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        screenQueue->insert(str);
        if (str == "DONE") {
            break;
        }
    }
}

void screenManager() {
    int done = 0;
    while (done < 3) {
        std::string str = screenQueue->remove();
        if (str == "DONE") {
            done++;
        } else {
            std::cout << str << std::endl;
        }
    }
    std::cout << "DONE" << std::endl;
}

std::vector<std::vector<int>> readFile(const std::string &file, int &coEditorQueue) {
    ifstream myFile;
    myFile.open(file);
    if (!myFile) {
        std::cout << file << " not found" << std::endl;
        exit(1);
    }
    std::ifstream in(file);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string contents = buffer.str();
    std::string space_delimiter = "\n";
    std::vector<std::string> words{};
    size_t pos;
    while ((pos = contents.find(space_delimiter)) != std::string::npos) {
        std::string word = contents.substr(0, pos);
        if (!word.empty()) {
            words.push_back(word);
        }
        contents.erase(0, pos + space_delimiter.length());
    }
    coEditorQueue = stoi(contents);
    std::vector<std::vector<int>> result;
    for (int i = 0; i < words.size(); i += 3) {
        result.emplace_back(std::vector<int>{stoi(words[i]), stoi(words[i + 1]), stoi(words[i + 2])});
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Configuration file not found" << std::endl;
        exit(1);
    }
    int coEditorQueue;
    std::vector<std::vector<int>> result = readFile(argv[1], coEditorQueue);
    coEditorQueues[0] = new UnboundedQueue();
    coEditorQueues[1] = new UnboundedQueue();
    coEditorQueues[2] = new UnboundedQueue();
    screenQueue = new BoundedQueue(3);
    producerQueues.resize(result.size());
    std::vector<std::thread> producers;
    for (int i = 0; i < result.size(); i++) {
        producerQueues[i] = new BoundedQueue(result[i][2]);
        producers.emplace_back(std::thread(producer, i, result[i][1]));
    }
    std::thread dispatcherThread(dispatcher);
    std::thread coEditorThread1(coEditor, 0);
    std::thread coEditorThread2(coEditor, 1);
    std::thread coEditorThread3(coEditor, 2);
    screenManager();
    for (int i = 0; i < result.size(); i++) {
        producers[i].join();
    }
    dispatcherThread.join();
    coEditorThread1.join();
    coEditorThread2.join();
    coEditorThread3.join();
    for (auto i: producerQueues) {
        delete i;
    }
    delete coEditorQueues[0];
    delete coEditorQueues[1];
    delete coEditorQueues[2];
    delete screenQueue;
}