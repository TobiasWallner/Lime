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

class HorizontalGrid : public RenderTrait{
	
	class Cell{
	public:
		using pointer = RenderTrait*;
		using unique_pointer = std::unique_ptr<RenderTrait>;
		using size_type = TermGui::ScreenWidth::size_type;
		
	private:
		enum class WidthType{Absolute, Relative, Dynamic};
		struct DynamicToken{};
		
		unique_pointer ownedElement = nullptr; // optionally own the object
		pointer element = nullptr;
		
		ScreenPosition screenPosition{0,0};
		ScreenWidth screenWidth{0,0};
		
		WidthType widthType = WidthType::Absolute;

		float relativeWidth = 0.0f;
		/// sets the minimal width of the cell, only works if the cell is relative
		size_type minimalWidth = 0;
		/// sets the maximal height of the cell, only works if the cell is relative
		size_type maximalWidth = std::numeric_limits<size_type>::max();
		
	public:
		static constexpr DynamicToken dynamicToken;
		static constexpr size_type maximalWidthLimit = std::numeric_limits<size_type>::max();
	
		inline Cell(){}

		/// constructs a grid cell with an absolute number of lines
		inline Cell(unique_pointer&& element, size_type absoluteWidth, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) : 
			ownedElement(std::move(element)), 
			element(element.get()),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = absoluteWidth, .y = 0}),
			widthType(WidthType::Absolute),
			relativeWidth(0.0f),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
			
		inline Cell(pointer element, size_type absoluteWidth, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) : 
			ownedElement(nullptr), 
			element(element),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = absoluteWidth, .y = 0}),
			widthType(WidthType::Absolute),
			relativeWidth(0.0f),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
			
		/// constructs a grid cell with a relative number of lines.
		/// This means that the number of lines of this cell is proportional 
		/// to other grid cells in the same grid
		inline Cell(unique_pointer&& element, float relativeWidth, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) : 
			ownedElement(std::move(element)), 
			element(element.get()),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			widthType(WidthType::Relative),
			relativeWidth(relativeWidth),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
			
		inline Cell(pointer element, float relativeWidth, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) : 
			ownedElement(nullptr), 
			element(element),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			widthType(WidthType::Relative),
			relativeWidth(relativeWidth),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
		
		/// constructs a grid cell with a dynamic number of lines.
		/// This means that the number of lines of an element in the cell is 
		/// defined by the element itself.
		inline Cell(unique_pointer&& element, DynamicToken dynamicToken, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) :  
			ownedElement(std::move(element)), 
			element(element.get()),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			widthType(WidthType::Absolute),
			relativeWidth(0.0f),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
			
		inline Cell(pointer element, DynamicToken dynamicToken, 
					size_type minimalWidth = 0, size_type maximalWidth = std::numeric_limits<size_type>::max()) : 
			ownedElement(nullptr), 
			element(element),
			screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
			screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
			widthType(WidthType::Absolute),
			relativeWidth(0.0f),
			minimalWidth(minimalWidth),
			maximalWidth(maximalWidth){}
		
		inline void set_maximal_width(size_type maximalWidth){ this->maximalWidth = maximalWidth;}
		inline void set_minimal_width(size_type minimalWidth){ this->minimalWidth = minimalWidth;}
		
		inline size_type get_maximal_width() const { return this->maximalWidth;}
		inline size_type get_minimal_width() const { return this->minimalWidth;}
		
		inline void set_grid_width_type(WidthType type){this->widthType = type;}
		
		inline WidthType get_grid_width_type() const {return widthType;}
		
		inline bool is_relative() const {return this->widthType == WidthType::Relative;}
		inline bool is_absolute() const {return this->widthType == WidthType::Absolute;}
		inline bool is_dynamic() const {return this->widthType == WidthType::Dynamic;}
		
		/// returns the absolute width wether it is valid or not
		/// check is_absolute() to know if it is valid
		inline ScreenWidth::size_type get_absolute_width() const {return this->screenWidth.x;}
		
		/// returns the relative width wether it is valid or not
		/// check is_relative to know if it is valid
		inline float get_relative_width() const {return this->relativeWidth;}
		
		/// returns the width of the stored object
		inline ScreenWidth::size_type get_dynamic_width() const {return this->element->get_screen_width().y;}
		
		/// returns the absolute width of grid cell if the cell is an absolute cell and 0 otherwise
		inline ScreenWidth::size_type get_width_if_absolute() const {return this->is_absolute() ? this->screenWidth.y : 0;}
		
		/// return the relatice width of the grid cell if the cell is a relatice cell and 0 otherwise
		inline float get_width_if_relative() const {return this->is_relative() ? this->relativeWidth : 0.0f;}
		
		/// return sthe dynamic width of the grid cell if the cell is a dynaic cell and 0 otherwise
		inline ScreenWidth::size_type get_width_if_dynamic() const {return this->is_dynamic() ? this->element->get_screen_width().y : 0;}
		
		/// makes the cell an absolute cell and sets the width
		inline void set_absolute_width(ScreenWidth::size_type width){
			this->widthType = WidthType::Absolute;
			this->screenWidth.y = width;
		}
		
		/// makes the cell a relative cell and sets the relative hight
		inline void set_relative_width(float width){
			this->widthType = WidthType::Relative;
			this->relativeWidth = width;
		}
		
		inline void set_dynamic_width(){
			this->widthType = WidthType::Dynamic;
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
	
	inline HorizontalGrid(){}
	
	inline HorizontalGrid(ScreenPosition screenPosition, ScreenWidth screenWidth, bool centering = false) : 
		gridCells(),
		screenPosition(screenPosition),
		screenWidth(screenWidth),
		centering(centering)
	{}
	
	/// inserts an element into the grid with an absolute width
	inline void insert_absolute(const_iterator pos, unique_pointer&& element, Cell::size_type width){
		this->gridCells.insert(pos, Cell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_absolute(const_iterator pos, pointer&& element, Cell::size_type width){
		this->gridCells.insert(pos, Cell(element, width));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, unique_pointer&& element, Cell::size_type width){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_absolute(size_type pos, pointer&& element, Cell::size_type width){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(element, width));
		this->distribute_cells();
	}


	/// inserts an element into the grid with a relative width
	/// note: 	that the relative width does not need to be a percentage, it can be a seen as a 'strength'
	/// 		if all elements have the relative width of 1 (or generally: the same relative-width), 
	///			then all will be equally spaced on the screen.
	inline void insert_relative(const_iterator pos, unique_pointer&& element, float width = 1.0f){
		this->gridCells.insert(pos, Cell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_relative(const_iterator pos, pointer&& element, float width = 1.0f){
		this->gridCells.insert(pos, Cell(element, width));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, unique_pointer&& element, float width = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(std::move(element), width));
		this->distribute_cells();
	}
	inline void insert_relative(size_type pos, pointer&& element, float width = 1.0f){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(element, width));
		this->distribute_cells();
	}
	
	
	inline void insert_dynamic(const_iterator pos, unique_pointer&& element){
		this->gridCells.insert(pos, Cell(std::move(element), Cell::dynamicToken));
		this->distribute_cells();
	}
	void insert_dynamic(const_iterator pos, pointer&& element){
		this->gridCells.insert(pos, Cell(element, Cell::dynamicToken));
		this->distribute_cells();
	}
	void insert_dynamic(size_type pos, unique_pointer&& element){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(std::move(element), Cell::dynamicToken));
		this->distribute_cells();
	}
	void insert_dynamic(size_type pos, pointer&& element){
		this->gridCells.insert(this->gridCells.cbegin() + pos, Cell(element, Cell::dynamicToken));
		this->distribute_cells();
	}
		
	/// appends an element with an absolute line number
	inline void push_back_absolute(unique_pointer&& element, ScreenWidth::size_type width){
		this->gridCells.push_back(Cell(std::move(element), width));
		this->distribute_cells();
	}
	inline void push_back_absolute(pointer element, ScreenWidth::size_type width){
		this->gridCells.push_back(Cell(element, width));
		this->distribute_cells();
	}

	inline void push_back_absolute_nodist(unique_pointer&& element, ScreenWidth::size_type width) {
		this->gridCells.push_back(Cell(std::move(element), width));
	}
	inline void push_back_absolute_nodist(pointer element, ScreenWidth::size_type width) {
		this->gridCells.push_back(Cell(element, width));
	}


	/// appends an element with a relative line number
	inline void push_back_relative(unique_pointer&& element, Cell::size_type minimalWidth = 0, Cell::size_type maximalWidth = Cell::maximalWidthLimit, float width = 1.0f){
		this->gridCells.push_back(Cell(std::move(element), width, minimalWidth, maximalWidth));
		this->distribute_cells();
	}
	inline void push_back_relative(pointer element, Cell::size_type minimalWidth = 0, Cell::size_type maximalWidth = Cell::maximalWidthLimit, float width = 1.0f){
		this->gridCells.push_back(Cell(element, width, minimalWidth, maximalWidth));
		this->distribute_cells();
	}

	inline void push_back_relative_nodist(unique_pointer&& element, Cell::size_type minimalWidth = 0, Cell::size_type maximalWidth = Cell::maximalWidthLimit, float width = 1.0f) {
		this->gridCells.push_back(Cell(std::move(element), width, minimalWidth, maximalWidth));
	}
	inline void push_back_relative_nodist(pointer element, Cell::size_type minimalWidth = 0, Cell::size_type maximalWidth = Cell::maximalWidthLimit, float width = 1.0f) {
		this->gridCells.push_back(Cell(element, width, minimalWidth, maximalWidth));
	}
		
	
	inline void push_back_dynamic(unique_pointer&& element){
		this->gridCells.push_back(Cell(std::move(element), Cell::dynamicToken));
		this->distribute_cells();
	}
	inline void push_back_dynamic(pointer element){
		this->gridCells.push_back(Cell(element, Cell::dynamicToken));
		this->distribute_cells();
	}

	inline void push_back_dynamic_nodist(unique_pointer&& element) {
		this->gridCells.push_back(Cell(std::move(element), Cell::dynamicToken));
	}
	inline void push_back_dynamic_nodist(pointer element) {
		this->gridCells.push_back(Cell(element, Cell::dynamicToken));
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
		for(const Cell& elem : this->gridCells){
			sum += elem.get_absolute_width();
		}
		return sum;
	}

	inline size_type accumulate_absolute_cell_width() const{
		ScreenWidth::size_type sum = 0;
		for(const Cell& elem : this->gridCells){
			sum += elem.get_width_if_absolute() + elem.get_width_if_dynamic();
		}
		return sum;
	}
		
	float accumulate_relative_cell_width() const{
		float sum = 0;
		for(const Cell& elem : this->gridCells){
			sum += elem.get_width_if_relative();
		}
		return sum;
	}
};
	
}