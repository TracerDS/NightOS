#include <init.hpp>
#include <cstdint>
#include <cstddef>
#include <memory/allocator.linked.hpp>

extern std::uint8_t* __free_memory_beginning__;
extern std::uint8_t* __free_memory_end__;

namespace Memory::Allocators::LinkedList {
#pragma pack(push, 1)
    struct Node {
        Node* prev;       // pointer to the previous free block
        Node* next;       // pointer to the next free block
        std::size_t size; // size of the block
    };
#pragma pack(pop)
    
    static Node* gs_head = nullptr;

    void Init() {
        auto beg = reinterpret_cast<std::uintptr_t>(__free_memory_beginning__);
        auto count = reinterpret_cast<std::uintptr_t>(__free_memory_end__) - beg;

        gs_head = reinterpret_cast<Node*>(beg);
        gs_head->size = count - sizeof(Node);
        gs_head->next = nullptr;
        gs_head->prev = nullptr;
    }

    void* kmalloc(std::size_t size) noexcept {
        if (size == 0) {
            return nullptr;
        }

        auto required_size = size + sizeof(Node);

        Node* current = gs_head;

        while (current) {
            if (current->size > required_size) {
                // Split the block
                auto remaining_size = current->size - required_size;
                current->size = remaining_size;

                Node* current_node = reinterpret_cast<Node*>(
                    reinterpret_cast<std::uintptr_t>(current) + current->size
                );
                current_node->size = size;
                current_node->prev = current;
                current_node->next = nullptr;

                return reinterpret_cast<void*>(
                    reinterpret_cast<std::uintptr_t>(current_node) + sizeof(Node)
                );
            }

            if (current->size < required_size) {
                current = current->next;
                continue;
            }

            // Exact fit

            if (!current->prev) {
                // We're at the head
                gs_head = current->next;
            } else {
                current->prev->next = current->next;
            }

            return reinterpret_cast<void*>(
                reinterpret_cast<std::uintptr_t>(current) + sizeof(Node)
            );
        }
        return nullptr;
    }

    void kfree(void* ptr) noexcept {
        if (!ptr) {
            return;
        }

        Node* block = reinterpret_cast<Node*>(
            reinterpret_cast<std::uintptr_t>(ptr) - sizeof(Node)
        );

        // Link the previous block to the next block
        if (block->prev) {
            block->prev->next = block->next;
        } else {
            // We're at the head
            gs_head = block->next;
        }

        if (block->next) {
            block->next->prev = block->prev;
        }
    }
};