#pragma once

#include <limits>

#include "GridTrait.hpp"
#include "RenderTrait.hpp"

namespace TermGui{

/// GridCell stores some element and additional information for the grid it is in.
/// can store a 'unique pointer' to an object, that owns the memory,
/// or a 'raw pointer' that does not store anything
class GridCell{
public:
	using size_type = TermGui::ScreenWidth::size_type;
	enum class LengthType{Absolute, Relative};

private:
	GridTrait* _grid = nullptr;

	ScreenPosition screenPosition{0,0};
	ScreenWidth screenWidth{0,0};
	
	LengthType lengthType = LengthType::Absolute;
	union{
		// if absolute:
		ScreenWidth targetWidth{0,0}; 
		
		// if relative:
		struct{
			float relativeLength; 
			/// sets the minimal length of the cell, only works if the cell is relative
			size_type minimalLength;
			/// sets the maximal length of the cell, only works if the cell is relative
			size_type maximalLength;
		};
	};
	
public:
	/// creates an absolute grid cell with the given target width
	inline GridCell(ScreenWidth targetWidth = ScreenWidth{0,0}) : 
		_grid(nullptr),
		screenPosition{0,0},
		screenWidth{0,0},
		lengthType(LengthType::Absolute),
		targetWidth(targetWidth)
		{}
		
	inline GridCell(float relativeLength, size_type minimalLength = 0, size_type maximalLength = std::numeric_limits<size_type>::max()) : 
		_grid(nullptr),
		screenPosition{0,0},
		screenWidth{0,0},
		lengthType(LengthType::Relative),
		relativeLength(relativeLength),
		minimalLength(minimalLength),
		maximalLength(maximalLength)
		{}
		
	
	virtual ~GridCell(){}
	
	inline void grid(GridTrait* grid){ this->_grid = grid; }
	inline void notify_grid(){ if(_grid != nullptr) this->_grid->distribute_cells(); }
	
	inline void maximal_length(size_type maximalLength){ 
		this->maximalLength = maximalLength;
		this->notify_grid();
	}
	
	inline void minimal_length(size_type minimalLength){ 
		this->minimalLength = minimalLength;
		this->notify_grid();
	}
	
	inline size_type maximal_length() const { 
		return this->is_relative() ? this->maximalLength : std::numeric_limits<size_type>::max();
	}
	
	inline size_type minimal_length() const { 
		return this->is_relative() ? this->minimalLength : 0;
	}	
	
	inline LengthType length_type() const {return this->lengthType;}
	inline bool is_relative() const {return this->lengthType == LengthType::Relative;}
	inline bool is_absolute() const {return this->lengthType == LengthType::Absolute;}
	
	/// returns the relative length wether it is valid or not
	/// check is_relative to know if it is valid
	inline float get_relative_length() const {
		return this->is_relative() ? this->relativeLength : 0.f;
	}
	
	virtual void render(std::string& output) const = 0;
	
	/// only the grid should be able to call the following function for the position and width of the screen
	virtual void set_screen_position(ScreenPosition position);
	/// only the grid should be able to call the following function for the position and width of the screen
	inline ScreenPosition get_screen_position() const{ return this->screenPosition; }
	/// only the grid should be able to call the following function for the position and width of the screen
	virtual void set_screen_width(ScreenWidth width);
	/// only the grid should be able to call the following function for the position and width of the screen
	inline ScreenWidth get_screen_width() const{ return this->screenWidth; }
	
	/// setting an absolute target width will change the type of the grid cell to absolute
	virtual void set_target_width(ScreenWidth width){ 
		this->lengthType = LengthType::Absolute;
		this->targetWidth = width; 
		this->notify_grid();
	}
	
	ScreenWidth get_target_width() const{ return this->is_absolute() ? this->targetWidth : ScreenWidth{0, 0}; }
	
};


}