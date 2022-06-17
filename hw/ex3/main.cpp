#include <fstream>
#include <sstream>
#include "BoundedQueue.h"
#include "iostream"
#include "thread"
#include "vector"
#include "UnboundedQueue.h"

std::vector<BoundedQueue> queues;
UnboundedQueue sportsCoEditorQueue;
UnboundedQueue newsCoEditorQueue;
UnboundedQueue weatherCoEditorQueue;
BoundedQueue screenQueue(10);

void screenManager() {
    int doneCnt = 0;
    while (true) {
        std::string str = screenQueue.remove();
        std::cout << "ScreenManager: " << str << std::endl;
        if (str == "DONE") {
            ++doneCnt;
        }
        if (doneCnt == 3) {
            std::cout << "DONE" << std::endl;
            break;
        }
    }
}

void sportsCoEditor() {
    std::string str;
    while (true) {
        str = sportsCoEditorQueue.remove();
//        std::cout << "SportsCoEditor: " << str << std::endl;
        screenQueue.insert(str);
        if (str == "DONE") {
            break;
        }
    }
}

void newsCoEditor() {
    std::string str;
    while (true) {
        str = newsCoEditorQueue.remove();
        screenQueue.insert(str);
//        std::cout << "NewsCoEditor: " << str << std::endl;
        if (str == "DONE") {
            break;
        }
    }
}

void weatherCoEditor() {
    std::string str;
    while (true) {
        str = weatherCoEditorQueue.remove();
        screenQueue.insert(str);
//        std::cout << "WeatherCoEditor: " << str << std::endl;
        if (str == "DONE") {
            break;
        }
    }
}

//void sort(const std::string &message) {
//    if (message.find("Sports") != std::string::npos) {
//        sportsQueue.insert(message);
//    } else if (message.find("News") != std::string::npos) {
//        newsQueue.insert(message);
//    } else if (message.find("Weather") != std::string::npos) {
//        weatherQueue.insert(message);
//    }
//}

void dispatcher(int queueCount) {
    std::vector<int> closed_queues;
    int closed_queues_count = 0;
    for (int i = 0; i < queueCount; i++) {
        closed_queues.push_back(0);
    }
    while (true) {
        for (int i = 0; i < queues.size(); i++) {
            if (closed_queues[i]) {
                continue;
            }
            std::string str = queues[i].remove();
            if (str == "DONE") {
                closed_queues[i] = 1;
                ++closed_queues_count;
            } else {
                if (str.find("Sports") != std::string::npos) {
                    sportsCoEditorQueue.insert(str);
                } else if (str.find("News") != std::string::npos) {
                    newsCoEditorQueue.insert(str);
                } else if (str.find("Weather") != std::string::npos) {
                    weatherCoEditorQueue.insert(str);
                }
            }
        }
        if (closed_queues_count == queueCount) {
            sportsCoEditorQueue.insert("DONE");
            newsCoEditorQueue.insert("DONE");
            weatherCoEditorQueue.insert("DONE");
            break;
        }
    }
}

void producer(int id, int newsNum, int queueSize) {
    queues.push_back(BoundedQueue(queueSize));
    int i;
    for (i = 0; i < newsNum / 3; i += 1) {
        queues[id].insert("Producer " + std::to_string(id) + " Sports " + std::to_string(i));
    }
    for (i = 0; i < newsNum / 3; i += 1) {
        queues[id].insert("Producer " + std::to_string(id) + " News " + std::to_string(i));
    }
    for (i = 0; i < newsNum / 3; i += 1) {
        queues[id].insert("Producer " + std::to_string(id) + " Weather " + std::to_string(i));
    }
    int j = 3 - newsNum % 3;
    while (j < 3) {
        queues[id].insert("Producer " + std::to_string(id) + " Weather " + std::to_string(i + 2 - j));
        ++j;
    }
    queues[id].insert("DONE");
}

std::vector<std::vector<int>> readFile(const std::string &file, int &coEditorQueue) {
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

int main() {
//    UnboundedQueue sportsQueue;
//    UnboundedQueue newsQueue;
//    UnboundedQueue weatherQueue;
    std::thread producer1(producer, 0, 30, 7);
    int queue_num = 1;
    std::thread dispatcher1(dispatcher, queue_num);
    std::thread sportsCoEditor1(sportsCoEditor);
    std::thread newsCoEditor1(newsCoEditor);
    std::thread weatherCoEditor1(weatherCoEditor);
    std::thread screenManager1(screenManager);
    screenManager1.join();
    sportsCoEditor1.join();
    newsCoEditor1.join();
    weatherCoEditor1.join();
    producer1.join();
    dispatcher1.join();
    return 0;
}

//std::vector<std::vector<int>> config;
//int main() {
//    //read main.cpp
//    int coEditorQueue;
//    config = readFile("conf.txt",coEditorQueue);
//    BoundedQueue screenQueue(coEditorQueue);
//    std::vector<std::thread> producers;
////    for (auto & i : config) {
////        producers.emplace_back(std::thread(producer, i[0], i[1], i[2]));
////    }
//    std::thread producer1(producer,1,6,7);
//    std::thread dispatcher1(dispatcher);
//    std::thread sportsCoEditorThread(sportsCoEditor, std::ref(screenQueue));
//    std::thread newsCoEditorThread(newsCoEditor, std::ref(screenQueue));
//    std::thread weatherCoEditorThread(weatherCoEditor, std::ref(screenQueue));
//    screenManager(screenQueue);
//    sportsCoEditorThread.join();
//    newsCoEditorThread.join();
//    weatherCoEditorThread.join();
//    dispatcher1.join();
//    producer1.join();
////    for (auto & i : producers) {
////        i.join();
////    }
//}