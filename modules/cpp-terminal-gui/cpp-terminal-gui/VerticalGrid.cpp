
// C++ std
#include <cmath>
#include <algorithm>

// Project
#include "VerticalGrid.hpp"


void TermGui::VerticalGrid::distribute_cells(){
	const TermGui::ScreenWidth::size_type absoluteCellHeight = this->accumulate_cell_target_height();
	float relativeCellsHeight = this->accumulate_relative_cell_height();
	
	TermGui::ScreenWidth::size_type remainingAbsoluteScreenHeight = std::min(this->screenWidth.y, absoluteCellHeight);
	TermGui::ScreenWidth::size_type remainingRelativeScreenHeight = this->screenWidth.y - remainingAbsoluteScreenHeight;
	
	// +++++++++++++++++++ distribute the heights of all cells +++++++++++++++++
	for(TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
		if(cell.is_relative()){
			// calculate height of the relative cell
			const float relativeHeight = cell.get_relative_length() / relativeCellsHeight;
			const float absoluteHeight = remainingRelativeScreenHeight * relativeHeight;
			const TermGui::ScreenWidth::size_type roundedHeight = static_cast<ScreenWidth::size_type>(std::round(absoluteHeight));
			const TermGui::ScreenWidth::size_type clippedHeight = std::max(std::min(roundedHeight, cell.maximal_length()), cell.minimal_length());
			const TermGui::ScreenWidth::size_type assignedHeight = std::min(remainingRelativeScreenHeight, clippedHeight);
			
			// assign height
			cell.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, assignedHeight});
			
			// post - prozessing and preparation for the next iteration
			relativeCellsHeight -= relativeHeight;
			remainingRelativeScreenHeight -= assignedHeight;
		}else if(cell.is_absolute()){
			const TermGui::ScreenWidth::size_type y = std::min(cell.get_target_width().y, remainingAbsoluteScreenHeight);
			cell.set_screen_width(TermGui::ScreenWidth{this->screenWidth.x, y});
			remainingAbsoluteScreenHeight -= y;
		}
	}
	
	// +++++++++++++++++++ distribute the position of all cells +++++++++++++++++
	TermGui::ScreenPosition position = this->screenPosition;
	if(this->centering){
		position.y += remainingRelativeScreenHeight / 2;
	}
	
	for(TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
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
		const TermGui::ScreenWidth::size_type total_clear_height = this->screenWidth.y - this->accumulate_cell_screen_height();
		{// clear lines before the first cell
			const TermGui::ScreenPosition::size_type clearLineStart = this->screenPosition.y;
			const TermGui::ScreenPosition::size_type clearLineEnd = clearLineStart + (this->centering ? total_clear_height / 2 : 0);
			for(auto line = clearLineStart; line < clearLineEnd; ++line){
				outputString += Term::cursor_move(line, this->screenPosition.x);
				outputString.append(this->screenWidth.x, ' ');
			}
		}

		// render cells
		for(const TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
			const GridCell& cell = *pcell.get();
			cell.render(outputString);
		}

		{ // clear lines after the last cell	
			const TermGui::ScreenPosition::size_type clearLineEnd = this->screenPosition.y + this->screenWidth.y;
			const TermGui::ScreenPosition::size_type clearLineStart = clearLineEnd - (this->centering ? total_clear_height / 2 : total_clear_height);
			for(auto line = clearLineStart; line < clearLineEnd; ++line){
				outputString += Term::cursor_move(line, this->screenPosition.x);
				outputString.append(this->screenWidth.x, ' ');
			}
		}
	}
}

void TermGui::VerticalGrid::set_screen_position(TermGui::ScreenPosition position){
	this->screenPosition = position;
	for(TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		GridCell& cell = *pcell.get();
		cell.set_screen_position(position);
		position.y += cell.get_screen_width().y;
	}
}

void TermGui::VerticalGrid::set_screen_width(TermGui::ScreenWidth width){
	this->screenWidth = width;
	this->distribute_cells();
}

TermGui::ScreenWidth::size_type TermGui::VerticalGrid::accumulate_cell_screen_height() const {
	ScreenWidth::size_type sum = 0;
	for(const TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_screen_width().y;
	}
	return sum;
}

TermGui::ScreenWidth::size_type TermGui::VerticalGrid::accumulate_cell_target_height() const{
	ScreenWidth::size_type sum = 0;
	for(const TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_target_width().y;
	}
	return sum;
}
	
float TermGui::VerticalGrid::accumulate_relative_cell_height() const{
	float sum = 0;
	for(const TermGui::VerticalGrid::UniqueGridCell& pcell : this->gridCells){
		const GridCell& cell = *pcell.get();
		sum += cell.get_relative_length();
	}
	return sum;
}
