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

		Iterator end()   { return Iterator(this->data()+RowsAtCompileTime*ColsAtCompileTime); }

		ConstIterator begin() const { return ConstIterator(this->data()); }

		ConstIterator end() const { return ConstIterator(this->data()+RowsAtCompileTime*ColsAtCompileTime); }
	
		using InitType = std::initializer_list<Scalar>;
		using BaseType = Scalar;

		Matrix(InitType l) : Matrix(l.size(), 1){
      unsigned int i=0;
      for(const auto& elem : l)
        this->operator()(i++, 0)=elem;
    }

   constexpr static std::size_t size() noexcept {return RowsAtCompileTime*ColsAtCompileTime*sizeof(Scalar);}
