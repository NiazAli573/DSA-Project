#include <iostream>
#include <vector>
#include <string>
using namespace std;

class UserProfile {
public:
    int user_id;
    string name;
    string password;
    string role;
    vector<int> liked_videos;

    UserProfile(int id, const string& name, const string& password, const string& role)
        : user_id(id), name(name), password(password), role(role) {}

    void likeVideo(int video_id) {
        liked_videos.push_back(video_id);
    }
};

// Graph class to represent relationships between users
class Graph {
private:
    vector<vector<int>> adjacencyList;

public:
    Graph(int size) {
        adjacencyList.resize(size);
    }

    void addEdge(int user1, int user2) {
        adjacencyList[user1].push_back(user2);
        adjacencyList[user2].push_back(user1); // Assuming undirected graph
    }

    vector<int> getConnections(int user_id) {
        return adjacencyList[user_id];
    }
};

// Stack class for displaying user connections
class Stack {
private:
    vector<int> stack;

public:
    void push(int value) {
        stack.push_back(value);
    }

    int pop() {
        if (stack.empty()) {
            throw runtime_error("Stack underflow!");
        }
        int value = stack.back();
        stack.pop_back();
        return value;
    }

    bool isEmpty() const {
        return stack.empty();
    }
};

class HashMap {
private:
    struct Node {
        int key;
        UserProfile value;
        Node* next;
    };
    Node** table;
    int capacity;

public:
    HashMap(int cap = 100) : capacity(cap) {
        table = new Node*[capacity]();
    }

    int hash(int key) {
        return key % capacity;
    }

    void insert(int user_id, const UserProfile& user) {
        int index = hash(user_id);
        Node* newNode = new Node{user_id, user, nullptr};

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* temp = table[index];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    UserProfile* get(int user_id) {
        int index = hash(user_id);
        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->key == user_id) {
                return &temp->value;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    ~HashMap() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] table;
    }
};

int main() {
    HashMap userMap;
    int userIDCounter = 1;
    Graph userGraph(100); // Graph to represent relationships
    string username, password;
    int userType;

    cout << "Select user type: \n0 for Creator\n1 for User\nEnter your choice: ";
    cin >> userType;

    if (userType == 0) {
        string creatorPassword = "Mehdi";
        cout << "Enter Creator Name: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (password == creatorPassword) {
            UserProfile creator(userIDCounter++, username, password, "creator");
            userMap.insert(creator.user_id, creator);
            cout << "Creator profile created successfully!" << endl;
        } else {
            cout << "Incorrect password for creator!" << endl;
        }
    } else if (userType == 1) {
        cout << "Enter User Name: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        UserProfile user(userIDCounter++, username, password, "user");

        int likedVideoID;
        char anotherVideo = 'y';
        while (anotherVideo == 'y' || anotherVideo == 'Y') {
            cout << "Enter the video ID you want to like (1 for Video 1, 2 for Video 2, etc.): ";
            cin >> likedVideoID;
            user.likeVideo(likedVideoID);
            cout << "Do you want to like another video? (y/n): ";
            cin >> anotherVideo;
        }

        userMap.insert(user.user_id, user);
        cout << "User profile created successfully!" << endl;
    } else {
        cout << "Invalid selection! Please restart the program and choose 0 or 1." << endl;
    }

    // Add relationships to the graph
    char addRelation = 'y';
    while (addRelation == 'y' || addRelation == 'Y') {
        int user1, user2;
        cout << "Enter two user IDs to create a relationship (user1 user2): ";
        cin >> user1 >> user2;
        userGraph.addEdge(user1, user2);
        cout << "Do you want to add another relationship? (y/n): ";
        cin >> addRelation;
    }

    int userIDToSearch;
    cout << "Enter user ID to fetch details: ";
    cin >> userIDToSearch;

    UserProfile* fetchedUser = userMap.get(userIDToSearch);
    if (fetchedUser != nullptr) {
        cout << "User Found!" << endl;
        cout << "User ID: " << fetchedUser->user_id << endl;
        cout << "Name: " << fetchedUser->name << endl;
        cout << "Role: " << fetchedUser->role << endl;
        cout << "Liked Videos: ";
        for (int videoID : fetchedUser->liked_videos) {
            cout << videoID << " ";
        }
        cout << endl;

        // Display connections using stack
        Stack userStack;
        vector<int> connections = userGraph.getConnections(fetchedUser->user_id);
        cout << "Connections: ";
        for (int conn : connections) {
            userStack.push(conn);
        }
        while (!userStack.isEmpty()) {
            cout << userStack.pop() << " ";
        }
        cout << endl;

    } else {
        cout << "User not found!" << endl;
    }

    return 0;
}
