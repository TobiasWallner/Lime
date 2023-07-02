// C++ std
#include <cmath>
#include <algorithm>

// Project
#include "HorizontalGrid.hpp"


void TermGui::HorizontalGrid::distribute_cells(){
	const TermGui::ScreenWidth::size_type absoluteCellWidth = this->accumulate_absolute_cell_width();
	
	float relativeCellsWidth = this->accumulate_relative_cell_width();
	ScreenWidth::size_type remainingScreenWidth = this->screenWidth.x - absoluteCellWidth;
	
	// +++++++++++++++++++ distribute the width of all cells +++++++++++++++++

	for(Cell& cell : this->gridCells){
		if(cell.is_relative()){
			// calculate width of the relative cell
			const float relativeWidth = cell.get_relative_width() / relativeCellsWidth;
			const float absoluteWidth = remainingScreenWidth * relativeWidth;
			const TermGui::ScreenWidth::size_type roundedWidth = static_cast<ScreenWidth::size_type>(std::round(absoluteWidth));
			const TermGui::ScreenWidth::size_type clippedHeight = std::max(std::min(roundedWidth, cell.get_minimal_width()), cell.get_maximal_width());
			const TermGui::ScreenWidth::size_type assignedWidth = std::min(remainingScreenWidth, clippedHeight);
			
			// assign width
			cell.set_screen_width(TermGui::ScreenWidth{assignedWidth, this->screenWidth.y});
			
			// post - prozessing and preparation for the next iteration
			relativeCellsWidth -= relativeWidth;
			remainingScreenWidth -= assignedWidth;
		}else if(cell.is_absolute()){
			cell.set_screen_width(TermGui::ScreenWidth{cell.get_width_if_absolute(), this->screenWidth.y});
		}

	}
	
	// +++++++++++++++++++ distribute the position of all cells +++++++++++++++++
	ScreenPosition position = this->screenPosition;
	if(this->centering){
		position.x += remainingScreenWidth / 2;
	}
	
	for(Cell& cell : this->gridCells){
		cell.set_screen_position(position);
		position.x += cell.get_screen_width().x;
	}
}


void TermGui::HorizontalGrid::render(std::string& outputString) const{
	if(this->gridCells.empty()){
		for(int line = 0; line < this->screenWidth.y; ++line){
			outputString += Term::cursor_move(this->screenPosition.y + line, this->screenPosition.x);
			outputString.append(this->screenWidth.x, ' ');
		}
	}else {
		
		const auto total_clear_width = this->screenWidth.x - this->accumulate_cell_width();
		if(this->centering){// clear lines before the first cell
			const auto clear_width = total_clear_width / 2;
			for(auto line = 0; line < this->screenWidth.y; ++line){
				outputString += Term::cursor_move(this->screenPosition.y + line, this->screenPosition.x);
				outputString.append(clear_width, ' ');
			}
		}
		
		for(const Cell& cell : this->gridCells){
			cell.render(outputString);
		}
		
		{// clear lines before the first cell
			const auto clear_width = this->centering ? total_clear_width / 2 : total_clear_width;
			if(clear_width != 0){
				for(auto line = 0; line < this->screenWidth.y; ++line){
					outputString += Term::cursor_move(this->screenPosition.y + line, this->screenPosition.x + this->screenWidth.x - clear_width);
					outputString.append(clear_width, ' ');
				}	
			}			
		}
	}
}

void TermGui::HorizontalGrid::set_screen_position(TermGui::ScreenPosition position){
	const long x_delta = static_cast<long>(position.x) - (this->screenPosition.x);
	const long y_delta = static_cast<long>(position.y) - (this->screenPosition.y);
	this->screenPosition = position;
	for(Cell& cell : this->gridCells){
		auto cellPosition = cell.get_screen_position();
		cellPosition.x += x_delta;
		cellPosition.y += y_delta;
		cell.set_screen_position(cellPosition);
	}
}
	
TermGui::ScreenPosition TermGui::HorizontalGrid::get_screen_position() const{
	return this->screenPosition;
}

void TermGui::HorizontalGrid::set_screen_width(TermGui::ScreenWidth width){
	this->screenWidth = width;
	this->distribute_cells();
}

TermGui::ScreenWidth TermGui::HorizontalGrid::get_screen_width() const{
	return this->screenWidth;
}