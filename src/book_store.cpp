#include "book_store.hpp"

#include <algorithm>  // copy
#include <stdexcept>  // invalid_argument


using namespace std;


ResizeStorageStatus resize_storage(Book *&storage, int size, int new_capacity) {
  if (size < 0) return ResizeStorageStatus::NEGATIVE_SIZE;
  if (new_capacity <= size) return ResizeStorageStatus::INSUFFICIENT_CAPACITY;
  if (storage == nullptr) return ResizeStorageStatus::NULL_STORAGE;

  Book *new_storage = new Book [new_capacity]{};
  copy(storage, storage + size, new_storage);
  delete[] storage;
  storage = new_storage;
  return ResizeStorageStatus::SUCCESS;
}


BookStore::BookStore(const std::string &name) : name_{name} {
  if (name.empty()) {
    throw std::invalid_argument("BookStore::name must not be empty");
  }

  storage_capacity_ = kInitStorageCapacity;
  storage_ = new Book [storage_capacity_]{};
}


BookStore::~BookStore() {
    delete [] storage_;
    storage_ = nullptr;
    storage_size_ = 0;
    storage_capacity_ = 0;
}

void BookStore::AddBook(const Book &book) {
  if (storage_size_ == storage_capacity_) {
    const ResizeStorageStatus status = resize_storage_internal(storage_capacity_ + kCapacityCoefficient);
    if (status != ResizeStorageStatus::SUCCESS) return;
  }
  storage_size_++;
  storage_[storage_size_ - 1] = book;

}

// РЕАЛИЗОВАНО

const std::string &BookStore::GetName() const {
  return name_;
}

int BookStore::GetSize() const {
  return storage_size_;
}

int BookStore::GetCapacity() const {
  return storage_capacity_;
}

const Book *BookStore::GetBooks() const {
  return storage_;
}

ResizeStorageStatus BookStore::resize_storage_internal(int new_capacity) {
  // изменяем размеры хранилища с копированием старых данных в хранилище нового объема
  const ResizeStorageStatus status = resize_storage(storage_, storage_size_, new_capacity);

  // если все прошло ОК, то сохраняем новый объем хранилища в объекте
  if (status == ResizeStorageStatus::SUCCESS) {
    storage_capacity_ = new_capacity;
  }

  return status;
}