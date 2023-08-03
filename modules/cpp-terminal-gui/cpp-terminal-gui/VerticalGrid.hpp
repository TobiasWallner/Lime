#pragma once

// C++ std
#include <memory>
#include <vector>
#include <limits>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// Project
#include "RenderTrait.hpp"
#include "GridCell.hpp"

namespace TermGui{

class VerticalGrid : public RenderTrait{
	using list_type = std::vector<GridCell>;
	
	list_type gridCells;
	
	ScreenPosition screenPosition{0,0};
	ScreenWidth screenWidth{0,0};
	
	bool centering = false;
	
public:
	
	using pointer = GridCell::pointer;
	using unique_pointer = GridCell::unique_pointer;
	
	using size_type = list_type::size_type;
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	
	inline VerticalGrid(){}
	
	inline VerticalGrid(ScreenPosition screenPosition, ScreenWidth screenWidth, bool centering = false) : 
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		centering(centering)
	{}
	
	
	/// inserts an element into the grid with an absolute height
	inline void insert_absolute(const_iterator pos, unique_pointer&& element, GridCell::size_type height){
		this->gridCells.insert(pos, GridCell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_absolute(const_iterator pos, pointer&& element, GridCell::size_type height){
		this->gridCells.insert(pos, GridCell(element, height));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, unique_pointer&& element, GridCell::size_type height){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, pointer&& element, GridCell::size_type height){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(element, height));
		this->distribute_cells();
	}


	/// inserts an element into the grid with a relative height
	/// note: 	that the relative height does not need to be a percentage, it can be a seen as a 'strength'
	/// 		if all elements have the relative height of 1 (or generally: the same relative-height), 
	///			then all will be equally spaced on the screen.
	inline void insert_relative(const_iterator pos, unique_pointer&& element, float height = 1.0f){
		this->gridCells.insert(pos, GridCell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_relative(const_iterator pos, pointer&& element, float height = 1.0f){
		this->gridCells.insert(pos, GridCell(element, height));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, unique_pointer&& element, float height = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, pointer&& element, float height = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(element, height));
		this->distribute_cells();
	}
	
	/// appends an element with an absolute line number
	inline void push_back_absolute(unique_pointer&& element, ScreenWidth::size_type height){
		this->gridCells.push_back(GridCell(std::move(element), height));
		this->distribute_cells();
	}
	
	inline void push_back_absolute(pointer element, ScreenWidth::size_type height){
		this->gridCells.push_back(GridCell(element, height));
		this->distribute_cells();
	}

	


	/// appends an element with a relative line number
	
	inline void push_back_relative(unique_pointer&& element, GridCell::size_type minimalHeight = 0, GridCell::size_type maximalHeight = GridCell::maximalHeightLimit, float height = 1.0f){
		this->gridCells.push_back(GridCell(std::move(element), height, minimalHeight, maximalHeight));
		this->distribute_cells();
	}
	
	inline void push_back_relative(pointer element, GridCell::size_type minimalHeight = 0, GridCell::size_type maximalHeight = GridCell::maximalHeightLimit, float height = 1.0f){
		this->gridCells.push_back(GridCell(element, height, minimalHeight, maximalHeight));
		this->distribute_cells();
	}
		
	inline void push_back_absolute_nodist(pointer element, ScreenWidth::size_type height) { this->gridCells.push_back(GridCell(element, height)); }
	inline void push_back_absolute_nodist(unique_pointer&& element, ScreenWidth::size_type height) { this->gridCells.push_back(GridCell(std::move(element), height)); }
	inline void push_back_relative_nodist(unique_pointer&& element, GridCell::size_type minimalHeight = 0, GridCell::size_type maximalHeight = GridCell::maximalHeightLimit, float height = 1.0f) {
		this->gridCells.push_back(GridCell(std::move(element), height, minimalHeight, maximalHeight));
	}
	inline void push_back_relative_nodist(pointer element, GridCell::size_type minimalHeight = 0, GridCell::size_type maximalHeight = GridCell::maximalHeightLimit, float height = 1.0f) {
		this->gridCells.push_back(GridCell(element, height, minimalHeight, maximalHeight));
	}

	inline void set_centering(bool centering) { 
		if(this->centering != centering){
			this->centering = centering;
			this->distribute_cells();
		}
	}

	inline void set_centering_nodist(bool centering) {
			this->centering = centering;
	}
	
	inline bool is_centered() const { return this->centering; }
	
	void render(std::string& outputString) const override;
	void set_screen_position(ScreenPosition position) override;
	ScreenPosition get_screen_position() const override;
	void set_screen_width(ScreenWidth width) override;
	ScreenWidth get_screen_width() const override;
	
	void distribute_cells();

private:

	inline size_type accumulate_cell_height() const {
		ScreenWidth::size_type sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_absolute_height();
		}
		return sum;
	}

	inline size_type accumulate_absolute_cell_height() const{
		ScreenWidth::size_type sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_height_if_absolute();
		}
		return sum;
	}
		
	inline float accumulate_relative_cell_height() const{
		float sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_height_if_relative();
		}
		return sum;
	}
};

}