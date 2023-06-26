
// C++ std
#include <cmath>
#include <algorithm>

// Project
#include "VerticalGrid.hpp"

//------------------------ VerticalGridCell ---------------------


TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::unique_pointer&& element, 
											TermGui::ScreenWidth::size_type absoluteHeight) : 
		ownedElement(std::move(element)),
		element(ownedElement.get()),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = absoluteHeight},
		relativeHeight(0.0f),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Absolute){}
		
TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::pointer element, 
											TermGui::ScreenWidth::size_type absoluteHeight) : 
		ownedElement(),
		element(element),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = absoluteHeight},
		relativeHeight(0.0f),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Absolute){}
		
		
		
TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::unique_pointer&& element, 
											float relativeHeight) : 
		ownedElement(std::move(element)),
		element(ownedElement.get()),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = 0},
		relativeHeight(relativeHeight),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Relative){}
		
TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::pointer element, 
											float relativeHeight) : 
		ownedElement(),
		element(element),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = 0},
		relativeHeight(relativeHeight),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Relative){}
		
		

TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::unique_pointer&& element, 
											DynamicToken dynamicToken) : 
		ownedElement(std::move(element)),
		element(ownedElement.get()),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = 0},
		relativeHeight(0.0f),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Absolute){}
		
TermGui::VerticalGridCell::VerticalGridCell(TermGui::VerticalGridCell::pointer element, 
											DynamicToken dynamicToken) : 
		ownedElement(),
		element(element),
		screenPosition{.x = 0, .y = 0},
		screenWidth{.x = 0, .y = 0},
		relativeHeight(0.0f),
		minimalHeight(0),
		maximalHeight(-1),
		gridWidthType(GridWidthType::Dynamic){}

void TermGui::VerticalGridCell::render(std::string& output) const {
	this->element->render(output);
}

void TermGui::VerticalGridCell::set_screen_position(TermGui::ScreenPosition position){
	this->screenPosition = position;
	this->element->set_screen_position(position);
}
	
TermGui::ScreenPosition TermGui::VerticalGridCell::get_screen_position() const{
	return this->screenPosition;
}

void TermGui::VerticalGridCell::set_screen_width(TermGui::ScreenWidth width){
	this->screenWidth = width;
	this->element->set_screen_width(width);
}

TermGui::ScreenWidth TermGui::VerticalGridCell::get_screen_width() const{
	return this->screenWidth;
}



//------------------------ VerticalGrid ---------------------

TermGui::ScreenWidth::size_type TermGui::VerticalGrid::accumulate_absolute_cell_height() const{
	ScreenWidth::size_type sum = 0;
	for(const VerticalGridCell& elem : this->gridCells){
		sum += elem.get_height_if_absolute() + elem.get_height_if_dynamic();
	}
	return sum;
}
	
float TermGui::VerticalGrid::accumulate_relative_cell_height() const{
	float sum = 0;
	for(const VerticalGridCell& elem : this->gridCells){
		sum += elem.get_height_if_relative();
	}
	return sum;
}


void TermGui::VerticalGrid::distribute_cells(){
	const TermGui::ScreenWidth::size_type absoluteCellHeight = this->accumulate_absolute_cell_height();
	
	float relativeCellsHeight = this->accumulate_relative_cell_height();
	ScreenWidth::size_type remainingScreenHeight = this->screenWidth.y - absoluteCellHeight;
	
	ScreenPosition position = this->screenPosition;
	
	for(VerticalGridCell& elem : this->gridCells){
		if(elem.is_relative()){
			// calculate height of the relative cell
			const float relativeHeight = elem.get_relative_height() / relativeCellsHeight;
			const float absoluteHeight = remainingScreenHeight * relativeHeight;
			const TermGui::ScreenWidth::size_type roundedHeight = static_cast<ScreenWidth::size_type>(std::round(absoluteHeight));
			const TermGui::ScreenWidth::size_type assignedHeight = std::min(remainingScreenHeight, roundedHeight);
			
			// assign height
			elem.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, assignedHeight});
			
			// post - prozessing and preparation for the next iteration
			relativeCellsHeight -= relativeHeight;
			remainingScreenHeight -= assignedHeight;
		}else if(elem.is_absolute()){
			elem.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, elem.get_height_if_absolute()});
		}
		elem.set_screen_position(position);
		position.y += elem.get_screen_width().y;
	}
}




inline void TermGui::VerticalGrid::insert_absolute(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::unique_pointer&& element, 
		ScreenWidth::size_type height){
	this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}
inline void TermGui::VerticalGrid::insert_absolute(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::pointer&& element, 
		ScreenWidth::size_type height){
	this->gridCells.insert(pos, VerticalGridCell(element, height));
	this->distribute_cells();
}
inline void TermGui::VerticalGrid::insert_absolute(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::unique_pointer&& element, 
		ScreenWidth::size_type height){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}
inline void TermGui::VerticalGrid::insert_absolute(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::pointer&& element, 
		ScreenWidth::size_type height){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(element, height));
	this->distribute_cells();
}




void TermGui::VerticalGrid::insert_relative(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::unique_pointer&& element, 
		float height){
	this->gridCells.insert(pos, VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_relative(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::pointer&& element, 
		float height){
	this->gridCells.insert(pos, VerticalGridCell(element, height));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_relative(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::unique_pointer&& element, 
		float height){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_relative(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::pointer&& element, 
		float height){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(element, height));
	this->distribute_cells();
}




void TermGui::VerticalGrid::insert_dynamic(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::unique_pointer&& element){
	this->gridCells.insert(pos, VerticalGridCell(std::move(element), TermGui::dynamicToken));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_dynamic(
		TermGui::VerticalGrid::const_iterator pos, 
		TermGui::VerticalGrid::pointer&& element){
	this->gridCells.insert(pos, VerticalGridCell(element, TermGui::dynamicToken));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_dynamic(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::unique_pointer&& element){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(std::move(element), TermGui::dynamicToken));
	this->distribute_cells();
}

void TermGui::VerticalGrid::insert_dynamic(
		TermGui::VerticalGrid::size_type pos, 
		TermGui::VerticalGrid::pointer&& element){
	this->gridCells.insert(this->gridCells.cbegin() + pos, VerticalGridCell(element, TermGui::dynamicToken));
	this->distribute_cells();
}



void TermGui::VerticalGrid::push_back_absolute(TermGui::VerticalGrid::unique_pointer&& element, ScreenWidth::size_type height){
	this->gridCells.push_back(VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}
void TermGui::VerticalGrid::push_back_absolute(TermGui::VerticalGrid::pointer element, ScreenWidth::size_type height){
	this->gridCells.push_back(VerticalGridCell(element, height));
	this->distribute_cells();
}



void TermGui::VerticalGrid::push_back_relative(TermGui::VerticalGrid::unique_pointer&& element, float height){
	this->gridCells.push_back(VerticalGridCell(std::move(element), height));
	this->distribute_cells();
}
void TermGui::VerticalGrid::push_back_relative(TermGui::VerticalGrid::pointer element,  float height){
	this->gridCells.push_back(VerticalGridCell(element, height));
	this->distribute_cells();
}



void TermGui::VerticalGrid::push_back_dynamic(TermGui::VerticalGrid::unique_pointer&& element){
	this->gridCells.push_back(VerticalGridCell(std::move(element), TermGui::dynamicToken));
	this->distribute_cells();
}
void TermGui::VerticalGrid::push_back_dynamic(TermGui::VerticalGrid::pointer element){
	this->gridCells.push_back(VerticalGridCell(element, TermGui::dynamicToken));
	this->distribute_cells();
}
	


void TermGui::VerticalGrid::render(std::string& outputString) const{
	for(const VerticalGridCell& elem : this->gridCells){
		elem.render(outputString);
	}
}

void TermGui::VerticalGrid::set_screen_position(TermGui::ScreenPosition position){
	this->screenPosition = position;
	for(VerticalGridCell& elem : this->gridCells){
		elem.set_screen_position(position);
		position.y += elem.get_screen_width().y;
	}
}
	
TermGui::ScreenPosition TermGui::VerticalGrid::get_screen_position() const{
	return this->screenPosition;
}

void TermGui::VerticalGrid::set_screen_width(TermGui::ScreenWidth width){
	this->screenWidth = width;
	this->distribute_cells();
}

TermGui::ScreenWidth TermGui::VerticalGrid::get_screen_width() const{
	return this->screenWidth;
}