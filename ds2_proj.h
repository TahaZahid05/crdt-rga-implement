#ifndef DS2_PROJ
#define DS2_PROJ

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <optional>

using namespace std;


struct RGA_Node {
    std::string id; // Unique identifier for the character
    char value;     // The character value
    bool is_deleted; // Tombstone to mark deletion
    std::string prev_id; // Stores previous character

    RGA_Node(std::string id, char value, string prev_id = ""): id(id), value(value), is_deleted(false), prev_id(prev_id) {}
};

class RGA {
private:
    std::vector<RGA_Node> nodes; // The main array storing the text
    std::map<std::string, size_t> id_to_index; // Map to quickly find nodes by ID

public:
    // Insert a character at a specific position
    void insert(const std::string& id, char value, const string& prev_id = "") {
        size_t index = nodes.size();
        if (prev_id != "" && id_to_index.find(prev_id) != id_to_index.end()) {
            index = id_to_index[prev_id] + 1;
        }
        else {
            for (const auto& node: nodes){
                if (node.prev_id == id) {
                    index = id_to_index[node.id];
                }
            }
        }
        
        RGA_Node new_node(id, value, prev_id);
        nodes.insert(nodes.begin() + index, new_node);
        id_to_index[id] = index;

        // Update indices for subsequent nodes
        for (size_t i = index + 1; i < nodes.size(); ++i) {
            if (nodes[i].prev_id == prev_id){
                nodes[i].prev_id = new_node.id;
            }
            id_to_index[nodes[i].id] = i;
        }
    }

    // Delete a character by marking it as deleted (tombstone)
    void remove(const std::string& id) {
        if (id_to_index.find(id) != id_to_index.end()) {
            size_t index = id_to_index[id];
            for (int j = 0; j < nodes.size(); j++){
                if(nodes[j].prev_id == nodes[index].id){
                    RGA_Node tempNode = nodes[id_to_index[nodes[index].prev_id]];
                    while (tempNode.is_deleted) {
                        if (tempNode.prev_id == ""){
                            break;
                        }
                        tempNode = nodes[id_to_index[tempNode.prev_id]];
                    }
                    if(!tempNode.is_deleted){
                        nodes[j].prev_id = tempNode.id;
                    }
                    else{
                        nodes[j].prev_id = "";
                    }
                    break;
                }
            }
            nodes[index].is_deleted = true;
        }
    }

    // Search for a character by its ID
    std::optional<char> search(const std::string& id) const {
        if (id_to_index.find(id) != id_to_index.end()) {
            size_t index = id_to_index.at(id);
            if (!nodes[index].is_deleted) {
                return nodes[index].value;
            }
        }
        return std::nullopt;
    }

    // Merge another RGA into this one (used for synchronization)
    void merge(const RGA& other) {
        std::set<std::string> existing_ids;
        for (const auto& node : nodes) {
            existing_ids.insert(node.id);
        }

        for (const auto& other_node : other.nodes) {
            if (existing_ids.find(other_node.id) == existing_ids.end()) {
                insert(other_node.id, other_node.value, other_node.prev_id);
                if (other_node.is_deleted) {
                    remove(other_node.id);
                }
            }
        }
    }

    // Print the current state of the document (ignoring deleted characters)
    void print_document() const {
        for (const auto& node : nodes) {
            if (!node.is_deleted) {
                std::cout << node.value;
            }
        }
        std::cout << std::endl;
        //yo
    }
};


#endif