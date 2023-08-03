#pragma once

// C++ std
#include <memory>
#include <vector>
#include <limits>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// Project
#include "RenderTrait.hpp"

namespace TermGui{

class VerticalGrid : public RenderTrait{
	
	/// Cell stores some element and additional information for the grid it is in.
	/// can store a 'unique pointer' to an object, that owns the memory,
	/// or a 'raw pointer' that does not store anything
	class Cell{
	public:
		using pointer = RenderTrait*;
		using unique_pointer = std::unique_ptr<RenderTrait>;
		using size_type = TermGui::ScreenWidth::size_type;
		
	private:
		
		enum class HeightType{Absolute, Relative};
		
		unique_pointer ownedElement = nullptr; // optionally own the object
		pointer element = nullptr;
		
		ScreenPosition screenPosition{0,0};
		ScreenWidth screenWidth{0,0};
		
		HeightType heightType = HeightType::Absolute;

		float relativeHeight = 0.0f;
		/// sets the minimal height of the cell, only works if the cell is relative
		size_type minimalHeight = 0;
		/// sets the maximal height of the cell, only works if the cell is relative
		size_type maximalHeight = std::numeric_limits<size_type>::max();
	public:
		static constexpr size_type maximalHeightLimit = std::numeric_limits<size_type>::max();
	
		inline Cell(){}
	
		/// constructs a grid cell with an absolute number of lines
		inline Cell(unique_pointer&& element, size_type absoluteHeight, 
					size_type minimalHeight = 0, size_type maximalHeight = std::numeric_limits<size_type>::max()) : 
			ownedElement(std::move(element)), 
			element(element.get()),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = absoluteHeight }),
			heightType(HeightType::Absolute),
			relativeHeight(0.0f),
			minimalHeight(minimalHeight),
			maximalHeight(maximalHeight){}
			
		inline Cell(pointer element, size_type absoluteHeight, 
					size_type minimalHeight = 0, size_type maximalHeight = std::numeric_limits<size_type>::max()) : 
			ownedElement(nullptr), 
			element(element),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = absoluteHeight }),
			heightType(HeightType::Absolute),
			relativeHeight(0.0f),
			minimalHeight(minimalHeight),
			maximalHeight(maximalHeight){}
			
		/// constructs a grid cell with a relative number of lines.
		/// This means that the number of lines of this cell is proportional 
		/// to other grid cells in the same grid
		inline Cell(unique_pointer&& element, float relativeHeight, 
					size_type minimalHeight = 0, size_type maximalHeight = std::numeric_limits<size_type>::max()) : 
			ownedElement(std::move(element)), 
			element(element.get()),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			heightType(HeightType::Relative),
			relativeHeight(relativeHeight),
			minimalHeight(minimalHeight),
			maximalHeight(maximalHeight){}
			
		inline Cell(pointer element, float relativeHeight, 
					size_type minimalHeight = 0, size_type maximalHeight = std::numeric_limits<size_type>::max()) : 
			ownedElement(nullptr), 
			element(element),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			heightType(HeightType::Relative),
			relativeHeight(relativeHeight),
			minimalHeight(minimalHeight),
			maximalHeight(maximalHeight){}
			
			
		inline void set_maximal_height(size_type maximalHeight){ this->maximalHeight = maximalHeight;}
		inline void set_minimal_height(size_type minimalHeight){ this->minimalHeight = minimalHeight;}
		
		inline size_type get_maximal_height() const { return this->maximalHeight;}
		inline size_type get_minimal_height() const { return this->minimalHeight;}	
		
		inline void set_grid_width_type(HeightType type){this->heightType = type;}
		
		inline HeightType get_grid_width_type() const {return heightType;}
		
		inline bool is_relative() const {return this->heightType == HeightType::Relative;}
		inline bool is_absolute() const {return this->heightType == HeightType::Absolute;}
		
		/// returns the absolute height wether it is valid or not
		/// check is_absolute() to know if it is valid
		inline ScreenWidth::size_type get_absolute_height() const {return this->screenWidth.y;}
		
		/// returns the relative height wether it is valid or not
		/// check is_relative to know if it is valid
		inline float get_relative_height() const {return this->relativeHeight;}
		
		/// returns the absolute height of grid cell if the cell is an absolute cell and 0 otherwise
		inline ScreenWidth::size_type get_height_if_absolute() const {return this->is_absolute() ? this->screenWidth.y : 0;}
		
		/// return the relatice height of the grid cell if the cell is a relatice cell and 0 otherwise
		inline float get_height_if_relative() const {return this->is_relative() ? this->relativeHeight : 0.0f;}
		
		/// makes the cell an absolute cell and sets the height
		inline void set_absolute_height(ScreenWidth::size_type height){
			this->heightType = HeightType::Absolute;
			this->screenWidth.y = height;
		}
		
		/// makes the cell a relative cell and sets the relative hight
		inline void set_relative_height(float height){
			this->heightType = HeightType::Relative;
			this->relativeHeight = height;
		}
		
		inline void render(std::string& output) const { this->element->render(output); }
		inline void set_screen_position(ScreenPosition position){ this->screenPosition = position; this->element->set_screen_position(position); }
		inline ScreenPosition get_screen_position() const{ return this->screenPosition; }
		inline void set_screen_width(ScreenWidth width){ this->screenWidth = width; this->element->set_screen_width(width); }
		inline ScreenWidth get_screen_width() const{ return this->screenWidth; }
	};
	
	using list_type = std::vector<Cell>;
	
	list_type gridCells;
	
	ScreenPosition screenPosition{0,0};
	ScreenWidth screenWidth{0,0};
	
	bool centering = false;
	
public:
	
	using pointer = Cell::pointer;
	using unique_pointer = Cell::unique_pointer;
	
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
	inline void insert_absolute(const_iterator pos, unique_pointer&& element, Cell::size_type height){
		this->gridCells.insert(pos, Cell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_absolute(const_iterator pos, pointer&& element, Cell::size_type height){
		this->gridCells.insert(pos, Cell(element, height));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, unique_pointer&& element, Cell::size_type height){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, pointer&& element, Cell::size_type height){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(element, height));
		this->distribute_cells();
	}


	/// inserts an element into the grid with a relative height
	/// note: 	that the relative height does not need to be a percentage, it can be a seen as a 'strength'
	/// 		if all elements have the relative height of 1 (or generally: the same relative-height), 
	///			then all will be equally spaced on the screen.
	inline void insert_relative(const_iterator pos, unique_pointer&& element, float height = 1.0f){
		this->gridCells.insert(pos, Cell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_relative(const_iterator pos, pointer&& element, float height = 1.0f){
		this->gridCells.insert(pos, Cell(element, height));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, unique_pointer&& element, float height = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(std::move(element), height));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, pointer&& element, float height = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(element, height));
		this->distribute_cells();
	}
	
	/// appends an element with an absolute line number
	inline void push_back_absolute(unique_pointer&& element, ScreenWidth::size_type height){
		this->gridCells.push_back(Cell(std::move(element), height));
		this->distribute_cells();
	}
	
	inline void push_back_absolute(pointer element, ScreenWidth::size_type height){
		this->gridCells.push_back(Cell(element, height));
		this->distribute_cells();
	}

	


	/// appends an element with a relative line number
	
	inline void push_back_relative(unique_pointer&& element, Cell::size_type minimalHeight = 0, Cell::size_type maximalHeight = Cell::maximalHeightLimit, float height = 1.0f){
		this->gridCells.push_back(Cell(std::move(element), height, minimalHeight, maximalHeight));
		this->distribute_cells();
	}
	
	inline void push_back_relative(pointer element, Cell::size_type minimalHeight = 0, Cell::size_type maximalHeight = Cell::maximalHeightLimit, float height = 1.0f){
		this->gridCells.push_back(Cell(element, height, minimalHeight, maximalHeight));
		this->distribute_cells();
	}
		
	inline void push_back_absolute_nodist(pointer element, ScreenWidth::size_type height) { this->gridCells.push_back(Cell(element, height)); }
	inline void push_back_absolute_nodist(unique_pointer&& element, ScreenWidth::size_type height) { this->gridCells.push_back(Cell(std::move(element), height)); }
	inline void push_back_relative_nodist(unique_pointer&& element, Cell::size_type minimalHeight = 0, Cell::size_type maximalHeight = Cell::maximalHeightLimit, float height = 1.0f) {
		this->gridCells.push_back(Cell(std::move(element), height, minimalHeight, maximalHeight));
	}
	inline void push_back_relative_nodist(pointer element, Cell::size_type minimalHeight = 0, Cell::size_type maximalHeight = Cell::maximalHeightLimit, float height = 1.0f) {
		this->gridCells.push_back(Cell(element, height, minimalHeight, maximalHeight));
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
		for(const Cell& elem : this->gridCells){
			sum += elem.get_absolute_height();
		}
		return sum;
	}

	inline size_type accumulate_absolute_cell_height() const{
		ScreenWidth::size_type sum = 0;
		for(const Cell& elem : this->gridCells){
			sum += elem.get_height_if_absolute();
		}
		return sum;
	}
		
	inline float accumulate_relative_cell_height() const{
		float sum = 0;
		for(const Cell& elem : this->gridCells){
			sum += elem.get_height_if_relative();
		}
		return sum;
	}
};

}