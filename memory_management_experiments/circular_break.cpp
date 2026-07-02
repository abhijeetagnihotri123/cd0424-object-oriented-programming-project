#include <iostream>
#include <string>
#include <memory>

class Parent; // Forward declaration

class Child {
public:
    std::weak_ptr<Parent> parent; // Use a weak_ptr to break the cycle
    ~Child() {
        std::cout << "Child destroyed." << std::endl;
    }
};

class Parent {
public:
    std::shared_ptr<Child> child;
    ~Parent() {
        std::cout << "Parent destroyed." << std::endl;
    }
};

int main() {
    std::shared_ptr<Parent> parent_ptr = std::make_shared<Parent>();
    std::shared_ptr<Child> child_ptr = std::make_shared<Child>();
    
    // Create the shared ownership and the weak link
    parent_ptr->child = child_ptr;
    child_ptr->parent = parent_ptr; // The weak_ptr does not increase the reference count

    // Now, let's demonstrate using the weak_ptr
    std::cout << "Parent's child use count: " << parent_ptr.use_count() << std::endl;
    std::cout << "Child's parent use count (via shared_ptr): " << child_ptr->parent.use_count() << std::endl;

    // Safely access the parent object from the child
    if (auto p = child_ptr->parent.lock()) {
        std::cout << "Successfully locked the parent object!" << std::endl;
    } else {
        std::cout << "The parent object has already been destroyed." << std::endl;
    }

    // Now, when parent_ptr and child_ptr go out of scope, the destructors will be called.
    return 0;
}