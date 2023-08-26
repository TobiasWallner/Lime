#pragma once

// C++ std
#include <memory>
#include <vector>
#include <limits>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// Project
#include "RenderTrait.hpp"
#include "GridTrait.hpp"
#include "GridCell.hpp"

namespace TermGui{

class VerticalGrid : public GridCell, public GridTrait{
public:
	using UniqueGridCell = std::unique_ptr<GridCell>;
	using list_type = std::vector<UniqueGridCell>;
	using size_type = list_type::size_type;
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;

private:
	list_type gridCells;
	ScreenPosition screenPosition{0,0};
	ScreenWidth screenWidth{0,0};
	bool centering = false;
	
public:
	inline VerticalGrid(){}
	
	inline VerticalGrid(ScreenPosition screenPosition, ScreenWidth screenWidth, bool centering = false) : 
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		centering(centering)
	{}
	
	inline void insert(const_iterator pos, UniqueGridCell gridCell){
		gridCell->grid(this);
		this->gridCells.insert(pos, std::move(gridCell));
		this->distribute_cells();
	}
		
	/// appends an element with an absolute line number
	inline void push_back(UniqueGridCell gridCell){
		gridCell->grid(this);
		this->gridCells.push_back(std::move(gridCell));
		this->distribute_cells();
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
	void set_screen_width(ScreenWidth width) override;
	
	void distribute_cells() override;

private:
	ScreenWidth::size_type accumulate_cell_screen_height() const;
	ScreenWidth::size_type accumulate_cell_target_height() const;	
	float accumulate_relative_cell_height() const;
};

}