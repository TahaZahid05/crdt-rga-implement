#include <iostream>
#include "ds2_proj.h"

using namespace std;

void run_tests() {
    RGA rga;

    // 1. Basic Insertions
    rga.insert("A1", 'H');
    rga.insert("A2", 'e', "A1");
    rga.insert("A3", 'l', "A2");
    rga.insert("A4", 'l', "A3");
    rga.insert("A5", 'o', "A4");
    std::cout << "Basic Insertions: "; rga.print_document();  // Expected: Hello

    // 2. Insertion in the Middle
    rga.insert("A6", 'X', "A1");
    std::cout << "Insertion in the Middle: "; rga.print_document();  // Expected: HXello

    // 3. Insertion at the End
    rga.insert("A7", '!', "A5");
    std::cout << "Insertion at the End: "; rga.print_document();  // Expected: HXello!

    // 4. Insertion Without prev_id (Append to End)
    rga.insert("A8", 'Y');
    std::cout << "Insertion Without prev_id: "; rga.print_document();  // Expected: HXello!Y

    // 5. Deleting a Character
    rga.remove("A2");
    std::cout << "After Deleting 'e': "; rga.print_document();  // Expected: HXllo!Y

    // 6. Deleting the First Character
    rga.remove("A1");
    std::cout << "After Deleting 'H': "; rga.print_document();  // Expected: Xllo!Y

    // 7. Deleting the Last Character
    rga.remove("A8");
    std::cout << "After Deleting 'Y': "; rga.print_document();  // Expected: Xllo!

    // 8. Searching for Existing and Non-Existing Characters
    auto result = rga.search("A3");
    std::cout << "Search A3: " << result.value_or(' ') << std::endl;  // Expected: l
    result = rga.search("Z1");
    std::cout << "Search Z1: " << result.value_or(' ') << std::endl;  // Expected: ' ' (not found)

    // 9. Merging Two CRDTs
    RGA rga2;
    rga2.insert("B1", 'W');
    rga2.insert("B2", 'o', "B1");
    rga2.insert("B3", 'r', "B2");
    rga2.insert("B4", 'l', "B3");
    rga2.insert("B5", 'd', "B4");
    rga.merge(rga2);
    std::cout << "After Merging with 'World': "; rga.print_document();  // Expected: Xllo!World

    // 10. Merging with Concurrent Edits
    RGA rga3;
    rga3.insert("C1", 'A');
    rga3.insert("C2", 'B', "C1");
    rga3.insert("C3", 'C', "C2");
    rga.merge(rga3);
    std::cout << "After Concurrent Merging: "; rga.print_document();  // Expected: Xllo!WorldABC

    // 11. Insert at an Invalid prev_id (Should Append to End)
    rga.insert("D1", 'Q', "NonExistentID");
    std::cout << "After Invalid Insert: "; rga.print_document();  // 'Q' should append at end
}

int main() {
    run_tests();
}