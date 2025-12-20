#include <cassert>
#include <ranges>

template <class T>
class Grid final {
public:
    using ValueType = T;
    using SizeType = std::uint32_t;

public:
    Grid(ValueType* data, SizeType y_size, SizeType x_size) : data_{new ValueType[y_size * x_size]}, y_size_{y_size}, x_size_{x_size} {
        for (SizeType index : std::ranges::iota_view{SizeType{0}, y_size_ * x_size_}) {
            data_[index] = data[index];
        }
    }

    Grid(const ValueType& value) : data_{new ValueType[1]{value}}, y_size_{1}, x_size_{1} {
    }

    Grid(SizeType y_size, SizeType x_size) : data_{new ValueType[y_size * x_size]{}}, y_size_{y_size}, x_size_{x_size} {
    }

    Grid(SizeType y_size, SizeType x_size, const ValueType& value) : data_{new ValueType[y_size * x_size]{value}}, y_size_{y_size}, x_size_{x_size} {
    }

    Grid(const Grid<ValueType>& other) : data_{new ValueType[other.y_size_ * other.x_size_]}, y_size_{other.y_size_}, x_size_{other.x_size_} {
        for (SizeType index : std::ranges::iota_view{SizeType{0}, y_size_ * x_size_}) {
            data_[index] = other.data_[index];
        }
    }

    Grid(Grid<ValueType>&& other) : data_{std::exchange(other.data_, nullptr)}, y_size_{std::move(other.y_size_)}, x_size_{std::move(other.x_size_)} {
    }

    Grid<ValueType>& operator=(const Grid<ValueType>& other) {
        if (this != &other) {
            if (data_ != nullptr) {
                delete[] data_;
            }
            data_ = new ValueType[other.y_size_ * other.x_size_];
            for (SizeType index : std::ranges::iota_view{SizeType{0}, y_size_ * x_size_}) {
                data_[index] = other.data_[index];
            }
            y_size_ = other.y_size_;
            x_size_ = other.x_size_;
        }
        return *this;
    }

    Grid<ValueType>& operator=(Grid<ValueType>&& other) {
        if (this != &other) {
            if (data_ != nullptr) {
                delete[] data_;
            }
            data_ = std::exchange(other.data_, nullptr);
            y_size_ = std::move(other.y_size_);
            x_size_ = std::move(other.x_size_);
        }
        return *this;
    }

    ~Grid() {
        if (data_ != nullptr) {
            delete[] data_;
        }
    }

    ValueType* operator[](SizeType y_index) {
        return data_ + y_index * x_size_;
    }

    ValueType operator()(SizeType y_index, SizeType x_index) const {
        return data_[y_index * x_size_ + x_index];
    }

    ValueType& operator()(SizeType y_index, SizeType x_index) {
        return data_[y_index * x_size_ + x_index];
    }

    // Grid<ValueType>& operator=(const ValueType& value) {
    //     for (SizeType index : std::ranges::iota_view{SizeType{0}, y_size_ * x_size_}) {
    //         data_[index] = value;
    //     }
    //     return *this;
    // }

    SizeType GetYSize() const {
        return y_size_;
    }

    SizeType GetXSize() const {
        return x_size_;
    }

private:
    ValueType* data_;
    SizeType y_size_;
    SizeType x_size_;
};

int main() {
    Grid<float> grid{3, 2, 0.0f};
    assert(3 == grid.GetYSize());
    assert(2 == grid.GetXSize());

    using GridSizeType = Grid<float>::SizeType;

    for (GridSizeType y_index : std::ranges::iota_view{GridSizeType{0}, grid.GetYSize()}) {
        for (GridSizeType x_index : std::ranges::iota_view{GridSizeType{0}, grid.GetXSize()}) {
            assert(0.0f == grid[y_index][x_index]);
        }
    }

    for (GridSizeType y_index : std::ranges::iota_view{GridSizeType{0}, grid.GetYSize()}) {
        for (GridSizeType x_index : std::ranges::iota_view{GridSizeType{0}, grid.GetXSize()}) {
            grid[y_index][x_index] = 1.0f;
        }
    }

    for (GridSizeType y_index : std::ranges::iota_view{GridSizeType{0}, grid.GetYSize()}) {
        for (GridSizeType x_index : std::ranges::iota_view{GridSizeType{0}, grid.GetXSize()}) {
            assert(1.0f == grid[y_index][x_index]);
        }
    }

    return 0;
}