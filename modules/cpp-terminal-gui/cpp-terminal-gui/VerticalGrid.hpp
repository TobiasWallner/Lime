
// C++ std
#include <cinttypes>
#include <memory>
#include <vector>
#include <variant>

// cpp-terminal
#include <cpp-terminal/cursor.hpp>

// project
#include "RenderTrait.hpp"


namespace TermGui{


enum class GridWidthType{
	Absolute,	// set a defined number of lines
	Relative,	// set a relative unber of lines that will be calculated from the remaining ones
	Dynamic		// askes the element underneath how many lines it needs
};

/// Token class with no memory that symbolises that a passed function parameter is dynamic
class DynamicToken{};

/// Token instance which if passed to a function symbolises that the other parameter is dynamic
inline constexpr DynamicToken dynamicToken;

/// VerticalGridCell stores some element and additional information for the grid it is in.
/// can store a 'unique pointer' to an object, that owns the memory,
/// or a 'raw pointer' that does not store anything
class VerticalGridCell : public RenderTrait{
public:
	using pointer = RenderTrait*;
	using unique_pointer = std::unique_ptr<RenderTrait>;
	using size_type = unsigned short;
	

private:
	unique_pointer ownedElement; // optionally store the object
	pointer element;
	
	ScreenPosition screenPosition;
	ScreenWidth screenWidth;
	
	GridWidthType gridWidthType;
	
	float relativeHeight;
	size_type minimalHeight;
	size_type maximalHeight;
	
	
public:

	/// constructs a grid cell with an absolute number of lines
	VerticalGridCell(unique_pointer&& element, ScreenWidth::size_type absoluteHeight);
	VerticalGridCell(pointer element, ScreenWidth::size_type absoluteHeight);
		
	/// constructs a grid cell with a relative number of lines.
	/// This means that the number of lines of this cell is proportional 
	/// to other grid cells in the same grid
	VerticalGridCell(unique_pointer&& element, float relativeHeight);
	VerticalGridCell(pointer element, float relativeHeight);
	
	/// constructs a grid cell with a dynamic number of lines.
	/// This means that the number of lines of an element in the cell is 
	/// defined by the element itself.
	VerticalGridCell(unique_pointer&& element, DynamicToken dynamicToken);
	VerticalGridCell(pointer element, DynamicToken dynamicToken);
		
	inline void set_grid_width_type(GridWidthType type){this->gridWidthType = type;}
	
	inline GridWidthType get_grid_width_type() const {return gridWidthType;}
	
	inline bool is_relative() const {return this->gridWidthType == GridWidthType::Relative;}
	inline bool is_absolute() const {return this->gridWidthType == GridWidthType::Absolute;}
	inline bool is_dynamic() const {return this->gridWidthType == GridWidthType::Dynamic;}
	
	/// returns the absolute height wether it is valid or not
	/// check is_absolute() to know if it is valid
	inline ScreenWidth::size_type get_absolute_height() const {return this->screenWidth.y;}
	
	/// returns the relative height wether it is valid or not
	/// check is_relative to know if it is valid
	inline float get_relative_height() const {return this->relativeHeight;}
	
	/// returns the height of the stored object
	inline ScreenWidth::size_type get_dynamic_height() const {return this->element->get_screen_width().y;}
	
	/// returns the absolute height of grid cell if the cell is an absolute cell and 0 otherwise
	inline ScreenWidth::size_type get_height_if_absolute() const {return this->is_absolute() ? this->screenWidth.y : 0;}
	
	/// return the relatice height of the grid cell if the cell is a relatice cell and 0 otherwise
	inline float get_height_if_relative() const {return this->is_relative() ? this->relativeHeight : 0.0f;}
	
	/// return sthe dynamic height of the grid cell if the cell is a dynaic cell and 0 otherwise
	inline ScreenWidth::size_type get_height_if_dynamic() const {return this->is_dynamic() ? this->element->get_screen_width().y : 0;}
	
	/// makes the cell an absolute cell and sets the height
	inline void set_absolute_height(ScreenWidth::size_type height){
		this->gridWidthType = GridWidthType::Absolute;
		this->screenWidth.y = height;
	}
	
	/// makes the cell a relative cell and sets the relative hight
	inline void set_relative_height(float height){
		this->gridWidthType = GridWidthType::Relative;
		this->relativeHeight = height;
	}
	
	inline void set_dynamic_height(){
		this->gridWidthType = GridWidthType::Dynamic;
	}
	
	inline void set_minimal_height(size_type minimalHeight) { this->minimalHeight = minimalHeight; }
	
	inline void set_maximal_height(size_type maximalHeight) { this->maximalHeight = maximalHeight; }
	
	void render(std::string& output) const override;
	
	void set_screen_position(ScreenPosition position) override;
	
	ScreenPosition get_screen_position() const override;
	
	void set_screen_width(ScreenWidth width) override;

	ScreenWidth get_screen_width() const override;
};

class VerticalGrid : public RenderTrait{
	
	using list_type = std::vector<VerticalGridCell>;
	
	list_type gridCells;
	
	ScreenPosition screenPosition;
	ScreenWidth screenWidth;
	
	bool centering = false;
	
public:
	
	using pointer = VerticalGridCell::pointer;
	using unique_pointer = VerticalGridCell::unique_pointer;
	
	using size_type = list_type::size_type;
	using iterator = list_type::iterator;
	using const_iterator = list_type::const_iterator;
	
	inline VerticalGrid(ScreenPosition screenPosition, ScreenWidth screenWidth) : 
		screenPosition(screenPosition),
		screenWidth(screenWidth)
	{}
	
	/// inserts an element into the grid with an absolute height
	void insert_absolute(const_iterator pos, unique_pointer&& element, ScreenWidth::size_type height);
	void insert_absolute(const_iterator pos, pointer&& element, ScreenWidth::size_type height);
	void insert_absolute(size_type pos, unique_pointer&& element, ScreenWidth::size_type height);
	void insert_absolute(size_type pos, pointer&& element, ScreenWidth::size_type height);
	
	/// inserts an element into the grid with a relative height
	/// note: 	that the relative height does not need to be a percentage, it can be a seen as a 'strength'
	/// 		if all elements have the relative height of 1 (or generally: the same relative-height), 
	///			then all will be equally spaced on the screen.
	void insert_relative(const_iterator pos, unique_pointer&& element, float height);
	void insert_relative(const_iterator pos, pointer&& element, float height);
	void insert_relative(size_type pos, unique_pointer&& element, float height);
	void insert_relative(size_type pos, pointer&& element, float height);
	
	void insert_dynamic(const_iterator pos, unique_pointer&& element);
	void insert_dynamic(const_iterator pos, pointer&& element);
	void insert_dynamic(size_type pos, unique_pointer&& element);
	void insert_dynamic(size_type pos, pointer&& element);
	
	/// appends an element with an absolute line number
	void push_back_absolute(unique_pointer&& element, ScreenWidth::size_type height);
	void push_back_absolute(pointer element, ScreenWidth::size_type height);
	
	/// appends an element with a relative line number
	void push_back_relative(unique_pointer&& element, float height = 1.0f);
	void push_back_relative(pointer element, float height = 1.0f);
	
	void push_back_dynamic(unique_pointer&& element);
	void push_back_dynamic(pointer element);
	
	void render(std::string& outputString) const override;
	
	/// sets the position of the object on the screen
	void set_screen_position(ScreenPosition position) override;
	
	/// get the position of the object on the screen
	ScreenPosition get_screen_position() const override;
	
	/// sets the width of the object on the screen
	void set_screen_width(ScreenWidth width) override;
	
	/// get the render width of the object
	ScreenWidth get_screen_width() const override;
	
	
private:

	ScreenWidth::size_type accumulate_absolute_cell_height() const;
	float accumulate_relative_cell_height() const;
	
	void distribute_cells();
	
};

}