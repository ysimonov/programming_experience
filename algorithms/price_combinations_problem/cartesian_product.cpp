#include <iostream>
#include <vector>

std::vector<std::vector<int>> cartesianProduct(std::vector<std::vector<int>> set_a, std::vector<int> set_b) {
    int i, j, k;
    int size_a = set_a.size();
    int size_b = set_b.size();
    std::vector<std::vector<int>> result;
    std::vector<int> temp;
    int num;
    result.clear();
    for(i=0; i<size_a; i++){
        for(j=0; j<size_b; j++){
            // coping all members of set_a to temp
            temp.clear();
            for(k=0; k<size_a; k++){
                for(auto el: set_a[i]){
                    temp.push_back(el);
                }
            }
            temp.push_back(set_b[j]);
            result.push_back(temp);
        }
    }
    return result;
}

void Cartesian(std::vector<::std::vector<int>> list_a, int n){
    // result of cartesian product of all the sets taken two at a time
    std::vector<std::vector<int>> temp;

    // for(int k=0; k<n; k++){
    //     for(int i=0; i<list_a[0].size(); i++){
    //         for(int j=1; j<list_a[i].size(); j++){
    //             temp.push_back(list_a[i]);
    //         }
    //     }
    // }

    temp.push_back(list_a[0]);

    // do product of N sets
    for(int i=1; i<n; i++) {
        temp = cartesianProduct(temp, list_a[i]);
    }

    for(auto k: temp) {
        for(auto l: k){
            std::cout << l << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    // vector input for cartesian product
    std::vector<std::vector<int>> list_a = {{1, 2, 3}, {4, 5}, {6, 9, 15}};
    // number of sets
    int n = list_a.size();
    std::cout << "Size of input vector: " << n << std::endl;
    // function is called to perform 
    // the cartesian product on list_a of size n
    Cartesian(list_a, n);
    return EXIT_SUCCESS;
}