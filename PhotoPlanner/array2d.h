#ifndef ARRAY2D_H
#define ARRAY2D_H

namespace aero_photo {

template <typename T, typename TVector = std::vector<T> >
    class Array2D {
public:
    Array2D(size_t rows, size_t columns) : rows_(rows), cols_(columns), data_(rows * columns) { }
    ~Array2D() { }

    auto Rows() const { return rows_; }
    auto Cols() const { return cols_; }

    auto &&Item (size_t row, size_t column) { return data_[CalculateIndex(row, column)]; }
    auto &&Item (size_t row, size_t column) const { return data_[CalculateIndex(row, column)]; }

    auto Data() { return data_.data(); }
    auto Data() const { return data_.data(); }

    void SetItems(auto &&setter) {
        for(size_t iRow = 0; iRow < rows_; iRow++)
            for(size_t iCol = 0; iCol < cols_; iCol++)
                Item(iRow, iCol) = setter(iRow, iCol);
    }
    void IterateItems (auto &&itemFun) const {
        for(size_t iRow = 0; iRow < rows_; iRow++)
            for(size_t iCol = 0; iCol < cols_; iCol++)
                itemFun(iRow, iCol, Item(iRow, iCol));
    }

private:
    inline size_t CalculateIndex(size_t row, size_t column) const { return row * cols_ + column; }

    const size_t rows_;
    const size_t cols_;
    TVector data_;
};

}

#endif // ARRAY2D_H
