#include "VariableSizeDeque.h"

struct CVariableSizeDeque::SImplementation {
    std::unique_ptr<std::any[]> DData;
    TDequeSize DCapacity;
    TDequeSize DSize;
    TDequeSize DFront;

    SImplementation()
        : DData(std::make_unique<std::any[]>(8)),
          DCapacity(8),
          DSize(0),
          DFront(0) {
    }

    void Grow() {
        TDequeSize newCapacity = DCapacity * 2;

        std::unique_ptr<std::any[]> newData =
            std::make_unique<std::any[]>(newCapacity);

        for (TDequeSize index = 0; index < DSize; index++) {
            TDequeSize oldIndex =
                (DFront + index) % DCapacity;

            newData[index] = std::move(DData[oldIndex]);
        }

        DData = std::move(newData);
        DCapacity = newCapacity;
        DFront = 0;
    }
};

CVariableSizeDeque::CVariableSizeDeque() {
    DImplementation = std::make_unique<SImplementation>();
}

CVariableSizeDeque::~CVariableSizeDeque() {
}

TDequeSize CVariableSizeDeque::Size() const {
    return DImplementation->DSize;
}

TDequeSize CVariableSizeDeque::MaxSize() const {
    return GDequeSizeVariable;
}

std::any CVariableSizeDeque::Front() const {
    if (DImplementation->DSize == 0) {
        return std::any();
    }

    return DImplementation->DData[DImplementation->DFront];
}

std::any CVariableSizeDeque::Back() const {
    if (DImplementation->DSize == 0) {
        return std::any();
    }

    TDequeSize backIndex =
        (DImplementation->DFront + DImplementation->DSize - 1)
        % DImplementation->DCapacity;

    return DImplementation->DData[backIndex];
}

bool CVariableSizeDeque::PushBack(std::any item) {
    if (DImplementation->DSize == DImplementation->DCapacity) {
        DImplementation->Grow();
    }

    TDequeSize backIndex =
        (DImplementation->DFront + DImplementation->DSize)
        % DImplementation->DCapacity;

    DImplementation->DData[backIndex] = std::move(item);
    DImplementation->DSize++;

    return true;
}

bool CVariableSizeDeque::PushFront(std::any item) {
    if (DImplementation->DSize == DImplementation->DCapacity) {
        DImplementation->Grow();
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

bool CVariableSizeDeque::PopBack() {
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

bool CVariableSizeDeque::PopFront() {
    if (DImplementation->DSize == 0) {
        return false;
    }

    DImplementation->DData[DImplementation->DFront].reset();

    DImplementation->DFront =
        (DImplementation->DFront + 1) % DImplementation->DCapacity;

    DImplementation->DSize--;

    return true;
}