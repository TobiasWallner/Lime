
namespace TermGui{

/// GridCell stores some element and additional information for the grid it is in.
/// can store a 'unique pointer' to an object, that owns the memory,
/// or a 'raw pointer' that does not store anything
class GridCell{
public:
	using pointer = RenderTrait*;
	using unique_pointer = std::unique_ptr<RenderTrait>;
	using size_type = TermGui::ScreenWidth::size_type;
	
private:
	
	enum class LengthType{Absolute, Relative};
	
	unique_pointer ownedElement = nullptr; // optionally own the object
	pointer element = nullptr;
	
	ScreenPosition screenPosition{0,0};
	ScreenWidth screenWidth{0,0};
	
	LengthType lengthType = LengthType::Absolute;

	float relativeLength = 0.0f;
	/// sets the minimal length of the cell, only works if the cell is relative
	size_type minimalLength = 0;
	/// sets the maximal length of the cell, only works if the cell is relative
	size_type maximalLength = std::numeric_limits<size_type>::max();
public:
	static constexpr size_type maximalLengthLimit = std::numeric_limits<size_type>::max();

	inline GridCell(){}

	/// constructs a grid cell with an absolute number of lines
	inline GridCell(unique_pointer&& element, size_type absoluteLength, 
				size_type minimalLength = 0, size_type maximalLength = std::numeric_limits<size_type>::max()) : 
		ownedElement(std::move(element)), 
		element(element.get()),
		screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
		screenWidth(TermGui::ScreenWidth{.x = 0, .y = absoluteLength }),
		lengthType(LengthType::Absolute),
		relativeLength(0.0f),
		minimalLength(minimalLength),
		maximalLength(maximalLength){}
		
	inline GridCell(pointer element, size_type absoluteLength, 
				size_type minimalLength = 0, size_type maximalLength = std::numeric_limits<size_type>::max()) : 
		ownedElement(nullptr), 
		element(element),
		screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
		screenWidth(TermGui::ScreenWidth{.x = 0, .y = absoluteLength }),
		lengthType(LengthType::Absolute),
		relativeLength(0.0f),
		minimalLength(minimalLength),
		maximalLength(maximalLength){}
		
	/// constructs a grid cell with a relative number of lines.
	/// This means that the number of lines of this cell is proportional 
	/// to other grid cells in the same grid
	inline GridCell(unique_pointer&& element, float relativeLength, 
				size_type minimalLength = 0, size_type maximalLength = std::numeric_limits<size_type>::max()) : 
		ownedElement(std::move(element)), 
		element(element.get()),
		screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
		screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
		lengthType(LengthType::Relative),
		relativeLength(relativeLength),
		minimalLength(minimalLength),
		maximalLength(maximalLength){}
		
	inline GridCell(pointer element, float relativeLength, 
				size_type minimalLength = 0, size_type maximalLength = std::numeric_limits<size_type>::max()) : 
		ownedElement(nullptr), 
		element(element),
		screenPosition(TermGui::ScreenPosition{.x = 0, .y = 0}),			
		screenWidth(TermGui::ScreenWidth{.x = 0, .y = 0}),
		lengthType(LengthType::Relative),
		relativeLength(relativeLength),
		minimalLength(minimalLength),
		maximalLength(maximalLength){}
		
		
	inline void set_maximal_length(size_type maximalLength){ this->maximalLength = maximalLength;}
	inline void set_minimal_length(size_type minimalLength){ this->minimalLength = minimalLength;}
	
	inline size_type get_maximal_length() const { return this->maximalLength;}
	inline size_type get_minimal_length() const { return this->minimalLength;}	
	
	inline void set_grid_width_type(LengthType type){this->lengthType = type;}
	
	inline LengthType get_grid_width_type() const {return lengthType;}
	
	inline bool is_relative() const {return this->lengthType == LengthType::Relative;}
	inline bool is_absolute() const {return this->lengthType == LengthType::Absolute;}
	
	/// returns the absolute length wether it is valid or not
	/// check is_absolute() to know if it is valid
	inline ScreenWidth::size_type get_absolute_length() const {return this->screenWidth.y;}
	
	/// returns the relative length wether it is valid or not
	/// check is_relative to know if it is valid
	inline float get_relative_length() const {return this->relativeLength;}
	
	/// returns the absolute length of grid cell if the cell is an absolute cell and 0 otherwise
	inline ScreenWidth::size_type get_length_if_absolute() const {return this->is_absolute() ? this->screenWidth.y : 0;}
	
	/// return the relatice length of the grid cell if the cell is a relatice cell and 0 otherwise
	inline float get_length_if_relative() const {return this->is_relative() ? this->relativeLength : 0.0f;}
	
	/// makes the cell an absolute cell and sets the length
	inline void set_absolute_length(ScreenWidth::size_type length){
		this->lengthType = LengthType::Absolute;
		this->screenWidth.y = length;
	}
	
	/// makes the cell a relative cell and sets the relative hight
	inline void set_relative_length(float length){
		this->lengthType = LengthType::Relative;
		this->relativeLength = length;
	}
	
	inline void render(std::string& output) const { this->element->render(output); }
	inline void set_screen_position(ScreenPosition position){ this->screenPosition = position; this->element->set_screen_position(position); }
	inline ScreenPosition get_screen_position() const{ return this->screenPosition; }
	inline void set_screen_width(ScreenWidth width){ this->screenWidth = width; this->element->set_screen_width(width); }
	inline ScreenWidth get_screen_width() const{ return this->screenWidth; }
};

}