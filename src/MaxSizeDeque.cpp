#include "MaxSizeDeque.h"

struct CMaxSizeDeque::SImplementation {
    std::unique_ptr<std::any[]> DData;
    TDequeSize DCapacity;
    TDequeSize DSize;
    TDequeSize DFront;

    explicit SImplementation(TDequeSize maxsize)
        : DData(maxsize > 0 ? std::make_unique<std::any[]>(maxsize) : nullptr),
          DCapacity(maxsize),
          DSize(0),
          DFront(0) {
    }
};

CMaxSizeDeque::CMaxSizeDeque(TDequeSize maxsize) {
    DImplementation = std::make_unique<SImplementation>(maxsize);
}

CMaxSizeDeque::~CMaxSizeDeque() {
}

TDequeSize CMaxSizeDeque::Size() const {
    return DImplementation->DSize;
}

TDequeSize CMaxSizeDeque::MaxSize() const {
    return DImplementation->DCapacity;
}

std::any CMaxSizeDeque::Front() const {
    if (DImplementation->DSize == 0) {
        return std::any();
    }

    return DImplementation->DData[DImplementation->DFront];
}

std::any CMaxSizeDeque::Back() const {
    if (DImplementation->DSize == 0) {
        return std::any();
    }

    TDequeSize backIndex =
        (DImplementation->DFront + DImplementation->DSize - 1)
        % DImplementation->DCapacity;

    return DImplementation->DData[backIndex];
}

bool CMaxSizeDeque::PushBack(std::any item) {
    if (DImplementation->DSize >= DImplementation->DCapacity) {
        return false;
    }

    TDequeSize backIndex =
        (DImplementation->DFront + DImplementation->DSize)
        % DImplementation->DCapacity;

    DImplementation->DData[backIndex] = std::move(item);
    DImplementation->DSize++;

    return true;
}

bool CMaxSizeDeque::PushFront(std::any item) {
    if (DImplementation->DSize >= DImplementation->DCapacity) {
        return false;
    }

    if (DImplementation->DSize == 0) {
        DImplementation->DFront = 0;
    } else {
        DImplementation->DFront =
            (DImplementation->DFront + DImplementation->DCapacity - 1)
            % DImplementation->DCapacity;
    }

    DImplementation->DData[DImplementation->DFront] = std::move(item);
    DImplementation->DSize++;

    return true;
}

bool CMaxSizeDeque::PopBack() {
    if (DImplementation->DSize == 0) {
        return false;
    }

    TDequeSize backIndex =
        (DImplementation->DFront + DImplementation->DSize - 1)
        % DImplementation->DCapacity;

    DImplementation->DData[backIndex].reset();
    DImplementation->DSize--;

    return true;
}

bool CMaxSizeDeque::PopFront() {
    if (DImplementation->DSize == 0) {
        return false;
    }

    DImplementation->DData[DImplementation->DFront].reset();

    DImplementation->DFront =
        (DImplementation->DFront + 1) % DImplementation->DCapacity;

    DImplementation->DSize--;

    return true;
}