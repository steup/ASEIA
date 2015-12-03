	class Iterator {
			Scalar *ptr;
			public:
				Iterator() = default;
				Iterator(Scalar *ptr) : ptr(ptr) {}
				Scalar& operator*() { return *ptr; }
				Iterator& operator++() { ++ptr; return *this;}
				Iterator  operator++(int) { Iterator temp(*this); ptr++; return temp;}
				bool operator==(const Iterator& b) const { return ptr == b.ptr; }
				bool operator!=(const Iterator& b) const { return ptr != b.ptr; }
		};

		class ConstIterator{
			const Scalar *ptr;
			public:
				ConstIterator() = default;
				ConstIterator(const Scalar* ptr) : ptr(ptr) {}
				const Scalar& operator*() const { return *ptr; }
				ConstIterator& operator++() { ++ptr; return *this;}
				ConstIterator  operator++(int) { ConstIterator temp(*this); ptr++; return temp;}
				bool operator==(const ConstIterator& b) const { return ptr == b.ptr; }
				bool operator!=(const ConstIterator& b) const { return ptr != b.ptr; }
		};

		Iterator begin() { return Iterator(this->data()); }

		Iterator end()   { return Iterator(this->data()+this->cols()*this->rows()); }

		ConstIterator begin() const { return ConstIterator(this->data()); }

		ConstIterator end() const { return ConstIterator(this->data()+this->cols()*this->rows()); }
	
		using RowInitType = std::initializer_list<Scalar>;
		using InitType    = std::initializer_list<RowInitType>;
		using BaseType    = Scalar;
		using U           = typename BaseType::U;
		using TypeID      = typename BaseType::TypeID;
		using Bool        = Matrix<ValueElement<bool, U::value>, RowsAtCompileTime, ColsAtCompileTime>;

		Matrix(InitType l) : Matrix(l.size(), l.begin()->size()){
      unsigned int i=0;
      for(const auto& row : l) {
				unsigned int j=0;
      	for(const auto& elem : row)
        	this->operator()(i, j++)=elem;
				i++;
			}
    }

		explicit operator ValueType() const {
			return ValueType(TypeID::value(), 
											 this->rows(), this->cols(), 
											 U::value);
		}

		Bool operator<( const Matrix& b ) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) < b(i, j);
			return res;
    }

		Bool operator>( const Matrix& b ) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) > b(i, j);
			return res;
    }

		Bool operator<=(const Matrix& b) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) <= b(i, j);
			return res;
		}

		Bool operator>=(const Matrix& b) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) >= b(i, j);
			return res;
		}

		Bool operator==(const Matrix& b) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) == b(i, j);
			return res;
		}

		Bool operator!=(const Matrix& b) const {
			Bool res(this->rows(), this->cols());
			for(unsigned int i=0;i<this->rows();i++)
				for(unsigned int j=0;j<this->cols();j++)
					res(i, j) = (*this)(i, j) != b(i, j);
			return res;
		}

		constexpr bool hasUncertainty() noexcept {return U::value;}

    constexpr static std::size_t staticSize() { return RowsAtCompileTime * ColsAtCompileTime * BaseType::size(); }
    std::size_t dynamicSize() const { return this->rows() * this->cols() * BaseType::size(); }
