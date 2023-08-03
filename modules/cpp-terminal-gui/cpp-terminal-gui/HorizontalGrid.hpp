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

class HorizontalGrid : public RenderTrait{
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
	
	inline HorizontalGrid(){}
	
	inline HorizontalGrid(ScreenPosition screenPosition, ScreenWidth screenWidth, bool centering = false) : 
		gridCells(),
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		centering(centering)
	{}
	
	/// inserts an element into the grid with an absolute width
	inline void insert_absolute(const_iterator pos, unique_pointer&& element, GridCell::size_type width){
		this->gridCells.insert(pos, GridCell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_absolute(const_iterator pos, pointer&& element, GridCell::size_type width){
		this->gridCells.insert(pos, GridCell(element, width));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, unique_pointer&& element, GridCell::size_type width){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, pointer&& element, GridCell::size_type width){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(element, width));
		this->distribute_cells();
	}


	/// inserts an element into the grid with a relative width
	/// note: 	that the relative width does not need to be a percentage, it can be a seen as a 'strength'
	/// 		if all elements have the relative width of 1 (or generally: the same relative-width), 
	///			then all will be equally spaced on the screen.
	inline void insert_relative(const_iterator pos, unique_pointer&& element, float width = 1.0f){
		this->gridCells.insert(pos, GridCell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_relative(const_iterator pos, pointer&& element, float width = 1.0f){
		this->gridCells.insert(pos, GridCell(element, width));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, unique_pointer&& element, float width = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, pointer&& element, float width = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, GridCell(element, width));
		this->distribute_cells();
	}
		
	/// appends an element with an absolute line number
	inline void push_back_absolute(unique_pointer&& element, ScreenWidth::size_type width){
		this->gridCells.push_back(GridCell(std::move(element), width));
		this->distribute_cells();
	}
	inline void push_back_absolute(pointer element, ScreenWidth::size_type width){
		this->gridCells.push_back(GridCell(element, width));
		this->distribute_cells();
	}

	inline void push_back_absolute_nodist(unique_pointer&& element, ScreenWidth::size_type width) {
		this->gridCells.push_back(GridCell(std::move(element), width));
	}
	inline void push_back_absolute_nodist(pointer element, ScreenWidth::size_type width) {
		this->gridCells.push_back(GridCell(element, width));
	}


	/// appends an element with a relative line number
	inline void push_back_relative(unique_pointer&& element, GridCell::size_type minimalWidth = 0, GridCell::size_type maximalWidth = GridCell::maximalLengthLimit, float width = 1.0f){
		this->gridCells.push_back(GridCell(std::move(element), width, minimalWidth, maximalWidth));
		this->distribute_cells();
	}
	inline void push_back_relative(pointer element, GridCell::size_type minimalWidth = 0, GridCell::size_type maximalWidth = GridCell::maximalLengthLimit, float width = 1.0f){
		this->gridCells.push_back(GridCell(element, width, minimalWidth, maximalWidth));
		this->distribute_cells();
	}

	inline void push_back_relative_nodist(unique_pointer&& element, GridCell::size_type minimalWidth = 0, GridCell::size_type maximalWidth = GridCell::maximalLengthLimit, float width = 1.0f) {
		this->gridCells.push_back(GridCell(std::move(element), width, minimalWidth, maximalWidth));
	}
	inline void push_back_relative_nodist(pointer element, GridCell::size_type minimalWidth = 0, GridCell::size_type maximalWidth = GridCell::maximalLengthLimit, float width = 1.0f) {
		this->gridCells.push_back(GridCell(element, width, minimalWidth, maximalWidth));
	}
		
	inline void set_centering(bool centering) { 
		if(this->centering != centering){
			this->centering = centering;
			this->distribute_cells();
		}
	}
	
	inline void set_centering_nodist(bool centering) {
		if (this->centering != centering) {
			this->centering = centering;
		}
	}

	inline bool is_centered() const { return this->centering; }
	
	void render(std::string& outputString) const override;
	void set_screen_position(ScreenPosition position) override;
	ScreenPosition get_screen_position() const override;
	void set_screen_width(ScreenWidth width) override;
	ScreenWidth get_screen_width() const override;
	
	void distribute_cells();

private:

	inline size_type accumulate_cell_width() const {
		ScreenWidth::size_type sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_absolute_length();
		}
		return sum;
	}

	inline size_type accumulate_absolute_cell_width() const{
		ScreenWidth::size_type sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_length_if_absolute();
		}
		return sum;
	}
		
	float accumulate_relative_cell_width() const{
		float sum = 0;
		for(const GridCell& elem : this->gridCells){
			sum += elem.get_length_if_relative();
		}
		return sum;
	}
};
	
}