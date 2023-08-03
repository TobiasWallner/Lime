
// C++ std
#include <cmath>
#include <algorithm>

// Project
#include "VerticalGrid.hpp"


void TermGui::VerticalGrid::distribute_cells(){
	const TermGui::ScreenWidth::size_type absoluteCellHeight = this->accumulate_absolute_cell_height();
	
	float relativeCellsHeight = this->accumulate_relative_cell_height();
	ScreenWidth::size_type remainingScreenHeight = this->screenWidth.y - absoluteCellHeight;
	
	// +++++++++++++++++++ distribute the heights of all cells +++++++++++++++++
	TermGui::ScreenWidth::size_type sum_height = 0;
	for(GridCell& cell : this->gridCells){
		if(cell.is_relative()){
			// calculate height of the relative cell
			const float relativeHeight = cell.get_relative_length() / relativeCellsHeight;
			const float absoluteHeight = remainingScreenHeight * relativeHeight;
			const TermGui::ScreenWidth::size_type roundedHeight = static_cast<ScreenWidth::size_type>(std::round(absoluteHeight));
			const TermGui::ScreenWidth::size_type clippedHeight = std::max(std::min(roundedHeight, cell.get_maximal_length()), cell.get_minimal_length());
			const TermGui::ScreenWidth::size_type assignedHeight = std::min(remainingScreenHeight, clippedHeight);
			
			// assign height
			cell.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, assignedHeight});
			
			// post - prozessing and preparation for the next iteration
			relativeCellsHeight -= relativeHeight;
			remainingScreenHeight -= assignedHeight;
		}else if(cell.is_absolute()){
			cell.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, cell.get_length_if_absolute()});
		}
		sum_height += cell.get_screen_width().y;
	}
	
	// +++++++++++++++++++ distribute the position of all cells +++++++++++++++++
	ScreenPosition position = this->screenPosition;
	if(this->centering){
		position.y += (this->screenWidth.y - sum_height) / 2;
	}
	
	for(GridCell& cell : this->gridCells){
		cell.set_screen_position(position);
		position.y += cell.get_screen_width().y;
	}
}


void TermGui::VerticalGrid::render(std::string& outputString) const{
	if(this->gridCells.empty()){
		for(int line = 0; line < this->screenWidth.y; ++line){
			outputString += Term::cursor_move(this->screenPosition.y + line, this->screenPosition.x);
			outputString.append(this->screenWidth.x, ' ');
		}
	}else {
		const auto total_clear_height = this->screenWidth.y - this->accumulate_cell_height();
		{// clear lines before the first cell
			const auto clearLineStart = this->screenPosition.y;
			const auto clearLineEnd = clearLineStart + (this->centering ? total_clear_height / 2 : 0);
			for(auto line = clearLineStart; line < clearLineEnd; ++line){
				outputString += Term::cursor_move(line, this->screenPosition.x);
				outputString.append(this->screenWidth.x, ' ');
			}
		}

		// render cells
		for(const GridCell& cell : this->gridCells){
			cell.render(outputString);
		}

		{ // clear lines after the last cell	
			const auto clearLineEnd = this->screenPosition.y + this->screenWidth.y;
			const auto clearLineStart = clearLineEnd - (this->centering ? total_clear_height / 2 : total_clear_height);
			for(auto line = clearLineStart; line < clearLineEnd; ++line){
				outputString += Term::cursor_move(line, this->screenPosition.x);
				outputString.append(this->screenWidth.x, ' ');
			}
		}
	}
}

void TermGui::VerticalGrid::set_screen_position(TermGui::ScreenPosition position){
	this->screenPosition = position;
	for(GridCell& cell : this->gridCells){
		cell.set_screen_position(position);
		position.y += cell.get_screen_width().y;
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