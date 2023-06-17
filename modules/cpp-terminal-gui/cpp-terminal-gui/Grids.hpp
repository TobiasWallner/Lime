
// C++ std
#include <memory>
#include <variant>
#include <cinttypes>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// project
#include "RenderTrait.hpp"


class VerticalGridCell{
public:
	using value_type = std::unique_ptr<RenderTrait>;

private:
	value_type element;
	ScreenSpace screenSpace;
	
	long x_pos;
	long y_pos;

	long height;
	long width;
	
	float relativeHeight;
	long minimalHeight;
	long maximalHeight;
	
	bool isAbsolute;
public:

	/// constructs an absolute grid cell
	VerticalGridCell(value_type&& element, long absoluteHeight) : 
		element(std::move(element)),
		absoluteHeight(absoluteHeight),
		relative height(0.0f),
		minimalHeight(-1),
		maximalHeight(-1),
		isAbsolute(true){}
		
	VerticalGridCell(value_type&& element, float relativeHeight) : 
		element(std::move(element)),
		absoluteHeight(0),
		relative height(relativeHeight),
		minimalHeight(-1),
		maximalHeight(-1),
		isAbsolute(false){}
		
	/// returns 'true' if the grid cell is absolute and false otherwise
	inline is_absolute() const {return this->isAbsolute;}
	
	/// return 'true' if the grid cell is relative and false otherwise
	inline is_relative() const {return !(this->is_relative);}
	
	/// returns the absolute height of grid cell if the cell is an absolute cell and 0 otherwise
	inline long get_height_if_absolute() const {return (this->isAbsolute) ? this->height : 0;}
	
	/// return the relatice height of the grid cell if the cell is a relatice cell and 0 otherwise
	inline float get_height_if_relative() const {return (!(this->isAbsolute)) ? this->relativeHeight : 0.0f;]}
	
	/// makes the cell an absolute cell and sets the height
	inline void set_absolute_height(long height){
		this->isAbsolute = true;
		this->height = height;
	}
	
	/// makes the cell a relative cell and sets the relative hight
	inline void set_relatice_height(float height){
		this->isAbsolute = false;
		this->relativeHeight = height;
	}
	
	inline void set_x_pos(long x){this->x_pos = x;}
	inline void set_y_pos(long y){this->y_pos = y;}
	
	inline void set_height(long height){this->height = height;}
	inline void set_width(long width){this->width = width;}
	
	
}

class VerticalGrid : public RenderTrait{
	
	using list_type = std::vector<VerticalGridCell>;
	
	list_type gridCells;
	
	public:
	
	using value_type = std::unique_ptr<RenderTrait>;
	using size_type = list_type::size_type;
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	
	VerticalGrid();
	
	/// inserts an element into the grid with an absolute height
	inline void insert_absolute(const_iterator pos, value_type&& element, long height){
		this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	}
	inline void insert_absolute(size_type pos, value_type&& element, long height){
		this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	}
	
	/// inserts an element into the grid with a relative height
	inline void insert_relative(const_iterator itr, value_type&& element, float height){
		this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	}
	inline void insert_relative(size_type itr, value_type&& element, float height){
		this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	}
	
	/// appends an element with an absolute line number
	inline void push_back_absolute(value_type&& element, long height){
		this->gridCells.push_back(VerticalGridCell(std::move(element), height));
	}
	
	/// appends an element with a relative line number
	inline void push_back_relative(value_type&& element, float height){
		this->gridCells.push_back(VerticalGridCell(std::move(element), height));
	}
	
}