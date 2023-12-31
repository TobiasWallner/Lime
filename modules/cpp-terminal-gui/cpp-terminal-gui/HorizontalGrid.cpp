// C++ std
#include <cmath>
#include <algorithm>

// Project
#include "HorizontalGrid.hpp"



void TermGui::HorizontalGrid::distribute_cells(){
	const TermGui::ScreenWidth::size_type absoluteCellWidth = this->accumulate_cell_target_width();
	float relativeCellsWidth = this->accumulate_relative_cell_width();
	
	TermGui::ScreenWidth::size_type remainingAbsoluteScreenWidth = std::min(this->GridCell::get_screen_width().x, absoluteCellWidth);
	TermGui::ScreenWidth::size_type remainingRelativeScreenWidth = this->GridCell::get_screen_width().x - remainingAbsoluteScreenWidth;
	
	// +++++++++++++++++++ distribute the width of all cells +++++++++++++++++
	for(TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
		if(cell.is_relative()){
			// calculate width of the relative cell
			const float relativeWidth = cell.get_relative_length() / relativeCellsWidth;
			const float absoluteWidth = remainingRelativeScreenWidth * relativeWidth;
			const TermGui::ScreenWidth::size_type roundedWidth = static_cast<ScreenWidth::size_type>(std::round(absoluteWidth));
			const TermGui::ScreenWidth::size_type clippedWidth = std::max(std::min(roundedWidth, cell.minimal_length()), cell.maximal_length());
			const TermGui::ScreenWidth::size_type assignedWidth = std::min(static_cast<size_type>(remainingRelativeScreenWidth), static_cast<size_type>(clippedWidth));
			
			// assign width
			cell.set_screen_width(TermGui::ScreenWidth{assignedWidth, this->GridCell::get_screen_width().y});
			
			// post - prozessing and preparation for the next iteration
			relativeCellsWidth -= relativeWidth;
			remainingRelativeScreenWidth -= assignedWidth;
		}else if(cell.is_absolute()){
			const TermGui::ScreenWidth::size_type x = std::min(cell.get_target_width().x, remainingAbsoluteScreenWidth);
			cell.set_screen_width(TermGui::ScreenWidth{x, this->GridCell::get_screen_width().y});
			remainingAbsoluteScreenWidth -= x;
		}
	}
	
	// +++++++++++++++++++ distribute the position of all cells +++++++++++++++++
	TermGui::ScreenPosition position = this->GridCell::get_screen_position();
	if(this->centering){
		position.x += remainingRelativeScreenWidth / 2;
	}
	
	for(TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
		cell.set_screen_position(position);
		position.x += cell.get_screen_width().x;
	}
}


void TermGui::HorizontalGrid::render(std::string& outputString) const{
	if(this->gridCells.empty()){
		for(TermGui::ScreenPosition::size_type line = 0; line < this->GridCell::get_screen_width().y; ++line){
			outputString += Term::cursor_move(this->GridCell::get_screen_position().y + line, this->GridCell::get_screen_position().x);
			outputString.append(this->GridCell::get_screen_width().x, ' ');
		}
	}else {
		
		if(this->centering){// clear lines before the first cell
			const TermGui::ScreenPosition::size_type x_from = this->GridCell::get_screen_position().x;
			const TermGui::ScreenPosition::size_type x_to = this->gridCells.front()->GridCell::get_screen_position().x;
			const TermGui::ScreenPosition::size_type clear_width = x_to - x_from;
			if (clear_width != 0) {
				for (auto line = 0; line < this->GridCell::get_screen_width().y; ++line) {
					outputString += Term::cursor_move(this->GridCell::get_screen_position().y + line, x_from);
					outputString.append(clear_width, ' ');
				}
			}
		}
		
		for(const TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
			const GridCell& cell = *pcell.get();
			cell.render(outputString);
		}
		
		{// clear lines before the first cell
			const TermGui::ScreenPosition::size_type x_from = this->gridCells.back()->GridCell::get_screen_position().x + this->gridCells.back()->get_screen_width().x;
			const TermGui::ScreenPosition::size_type x_to = this->GridCell::get_screen_position().x + this->GridCell::get_screen_width().x;
			const TermGui::ScreenPosition::size_type clear_width = x_to - x_from;
			if(clear_width != 0){
				for(auto line = 0; line < this->GridCell::get_screen_width().y; ++line){
					outputString += Term::cursor_move(this->GridCell::get_screen_position().y + line, x_from);
					outputString.append(clear_width, ' ');
				}	
			}			
		}
	}
}

void TermGui::HorizontalGrid::set_screen_position(TermGui::ScreenPosition position){
	const long x_delta = static_cast<long>(position.x) - (this->GridCell::get_screen_position().x);
	const long y_delta = static_cast<long>(position.y) - (this->GridCell::get_screen_position().y);
	this->GridCell::set_screen_position(position);
	for(TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
		auto cellPosition = cell.get_screen_position();
		cellPosition.x += x_delta;
		cellPosition.y += y_delta;
		cell.set_screen_position(cellPosition);
	}
}
	
void TermGui::HorizontalGrid::set_screen_width(TermGui::ScreenWidth width){
	this->GridCell::set_screen_width(width);
	this->distribute_cells();
}

TermGui::ScreenWidth::size_type TermGui::HorizontalGrid::accumulate_cell_screen_width() const {
	ScreenWidth::size_type sum = 0;
	for(const TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_screen_width().x;
	}
	return sum;
}

TermGui::ScreenWidth::size_type TermGui::HorizontalGrid::accumulate_cell_target_width() const{
	ScreenWidth::size_type sum = 0;
	for(const TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_target_width().x;
	}
	return sum;
}
	
float TermGui::HorizontalGrid::accumulate_relative_cell_width() const{
	float sum = 0;
	for(const TermGui::HorizontalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_relative_length();
	}
	return sum;
}