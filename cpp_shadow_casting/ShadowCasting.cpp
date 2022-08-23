/*
	OneLoneCoder_PGE_ShadowCasting2D.cpp
	What is this?
	~~~~~~~~~~~~~
	This is an implementation of two algorithms that work together
	to produuce a "line-of-sight" or "shadow casting" effect depending
	upon your perspective. To use it compile with olcPixelGameEngine.h,
	place/remove blocks with left click and hold down right mouse button
	to illuminate the scene from the mouse cursor.
	The first algorithm converts a tile map into a set of geometric 
	primitives whcih are more conveninet to use in geometric programs
	such as this. The second algorithm casts rays to create a triangle
	fan that represents the visible area form the source.
	Thanks to these resources for ideas
	https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	https://www.redblobgames.com/articles/visibility/
	https://ncase.me/sight-and-light/
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Relevant Videos
	~~~~~~~~~~~~~~~
	https://youtu.be/kRH6oJLFYxY Introducing olcPixelGameEngine
	--> https://youtu.be/fc3nnG2CG8U Video about this very code!
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/



#include <iostream>
#include <memory>

using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// compile: 
// g++ -o ShadowCasting ShadowCasting.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17



/* 
 * Struct containing x, y components of the start and end points
 * of a single segment (line segment)
 */
struct sSegment
{
    float sx, sy;
    float ex, ey;
};


/*
 * Each cell constitutes of 4 distinct line segments
 */
struct sCell
{
    // segment id in the pool of segments
    int segment_id[4];

    // does the segment exist on all 4 sides?
    bool segment_exists[4];

    // does the cell exist?
    bool exists = false;
};


enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};


class ShadowCasting2D : public olc::PixelGameEngine 
{
    public:
        ShadowCasting2D()
        {
            sAppName = "ShadowCasting2D";
        }

        bool OnUserCreate() override
        {
            pWorld = make_unique<sCell[]>(nWorldWidth * nWorldHeight);
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            float fBlockWidth = 16.0f;
            float fSourceX = GetMouseX();
            float fSourceY = GetMouseY();

            // Checks if block is pressed or released
            if (GetMouse(0).bReleased)
            {
                // i = y * width + x
                int i = int(fSourceY / fBlockWidth) * nWorldWidth +     // haw far vertically
                        int(fSourceX / fBlockWidth);                    // how far horizontally

                pWorld[i].exists = !pWorld[i].exists;

                ConvertTileMapToPolyMap(0, 0, 40, 30, fBlockWidth, nWorldWidth);
            }

            // Drawing

            // Clear the screen to all black
            Clear(olc::BLACK);


            // Draw Blocks from Tile Map
            for (int x = 0; x < nWorldWidth; x++)
            {
                for (int y = 0; y < nWorldHeight; y++) 
                {
                    if (pWorld[y * nWorldWidth + x].exists)
                    {
                        FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
                    }
                }
            }


            // Draw Edges
            for (auto& seg : vecSegments) {
                DrawLine(seg.sx, seg.sy, seg.ex, seg.ey);
                FillCircle(seg.sx, seg.sy, 3, olc::RED);
                FillCircle(seg.ex, seg.ey, 3, olc::RED);
            }

            return true;
        }

    private:
        unique_ptr<sCell[]> pWorld = nullptr;
        const int nWorldWidth = 40;
        const int nWorldHeight = 30;

        vector<sSegment> vecSegments;

        void ConvertTileMapToPolyMap(
            int sx, int sy,
            int w, int h,
            float fBlockWidth,
            int pitch   // world width
        )
        {
            // clear polymap
            vecSegments.clear();

            for (int x = 0; x < w; x++) 
            {
                for (int y = 0; y < h; y++) 
                {
                    for (int j = 0; j < 4; j++)
                    {
                        // no segments in the pool
                        auto& pW = pWorld[(y + sy) * pitch + (x + sx)];
                        pW.segment_exists[j] = false;
                        pW.segment_id[j] = 0;
                    }
                }
            }

            // iterate from top left to bottom right
            auto wm = w - 1;
            auto hm = h - 1;
    
            for (int x = 1; x < wm; x++) 
            {
                int xpsx = x + sx;
                for (int y = 1; y < hm; y++)
                {
                    int ypsy = y + sy;
    
                    // current index of cell in the array
                    int i = ypsy * pitch + xpsx;

                    // check if cell exists
                    if (!pWorld[i].exists) continue;


                    // northern neighbour index
                    int n = (ypsy - 1) * pitch + xpsx;

                    // southern neighbour index
                    int s = (ypsy + 1) * pitch + xpsx;

                    // western neighbour index
                    int w = ypsy * pitch + (xpsx - 1);

                    // eastern neighbour index
                    int e = ypsy * pitch + (xpsx + 1);


                    // *** WEST ***
                    // if the cell does not have the western segment
                    if (!pWorld[w].exists)
                    {
                        // nothern neighbour has a western segment
                        if (pWorld[n].segment_exists[WEST])
                        {
                            // extend the segment
                            vecSegments[pWorld[n].segment_id[WEST]].ey += fBlockWidth;
                            pWorld[i].segment_id[WEST] = pWorld[n].segment_id[WEST];
                            pWorld[i].segment_exists[WEST] = true;
                        }
                        // create a western segment
                        else
                        {
                            sSegment segment;

                            segment.sx = xpsx + fBlockWidth;
                            segment.sy = ypsy + fBlockWidth;
                            segment.ex = segment.sx;
                            segment.ey = segment.ey + fBlockWidth;

                            // add edge to polygon pool
                            int segment_id = vecSegments.size();
                            vecSegments.emplace_back(segment);

                            // update tile information
                            pWorld[i].segment_id[WEST] = segment_id;
                            pWorld[i].segment_exists[WEST] = true;
                        }
                    }

                    // *** EAST ***
                    // if the cell does not have the eastern segment
                    if (!pWorld[e].exists)
                    {
                        // nothern neighbour has a eastern segment
                        if (pWorld[n].segment_exists[EAST])
                        {
                            // extend the segment
                            vecSegments[pWorld[n].segment_id[EAST]].ey += fBlockWidth;
                            pWorld[i].segment_id[EAST] = pWorld[n].segment_id[EAST];
                            pWorld[i].segment_exists[EAST] = true;
                        }
                        // create a eastern segment
                        else
                        {
                            sSegment segment;

                            segment.sx = xpsx + 1 + fBlockWidth;
                            segment.sy = ypsy + fBlockWidth;
                            segment.ex = segment.sx;
                            segment.ey = segment.ey + fBlockWidth;

                            // add edge to polygon pool
                            int segment_id = vecSegments.size();
                            vecSegments.emplace_back(segment);

                            // update tile information
                            pWorld[i].segment_id[EAST] = segment_id;
                            pWorld[i].segment_exists[EAST] = true;
                        }
                    }


                    // *** NORTH ***
                    // if the cell does not have the northern segment
                    if (!pWorld[n].exists)
                    {
                        // nothern neighbour has a western segment
                        if (pWorld[w].segment_exists[NORTH])
                        {
                            // extend the segment
                            vecSegments[pWorld[w].segment_id[NORTH]].ex += fBlockWidth;
                            pWorld[i].segment_id[NORTH] = pWorld[w].segment_id[NORTH];
                            pWorld[i].segment_exists[NORTH] = true;
                        }
                        // create a northern segment
                        else
                        {
                            sSegment segment;

                            segment.sx = xpsx + fBlockWidth;
                            segment.sy = ypsy + fBlockWidth;
                            segment.ex = segment.sx + fBlockWidth;
                            segment.ey = segment.ey;

                            // add edge to polygon pool
                            int segment_id = vecSegments.size();
                            vecSegments.emplace_back(segment);

                            // update tile information
                            pWorld[i].segment_id[NORTH] = segment_id;
                            pWorld[i].segment_exists[NORTH] = true;
                        }
                    }


                    // *** SOUTH ***
                    // if the cell does not have the western segment
                    if (!pWorld[s].exists)
                    {
                        // nothern neighbour has a western segment
                        if (pWorld[w].segment_exists[SOUTH])
                        {
                            // extend the segment
                            vecSegments[pWorld[w].segment_id[SOUTH]].ex += fBlockWidth;
                            pWorld[i].segment_id[SOUTH] = pWorld[w].segment_id[SOUTH];
                            pWorld[i].segment_exists[SOUTH] = true;
                        }
                        // create a southern segment
                        else
                        {
                            sSegment segment;

                            segment.sx = xpsx + fBlockWidth;
                            segment.sy = ypsy + 1 + fBlockWidth;
                            segment.ex = segment.sx + fBlockWidth;
                            segment.ey = segment.ey;

                            // add edge to polygon pool
                            int segment_id = vecSegments.size();
                            vecSegments.emplace_back(segment);

                            // update tile information
                            pWorld[i].segment_id[SOUTH] = segment_id;
                            pWorld[i].segment_exists[SOUTH] = true;
                        }
                    }


                }
            }
        }
};



int main()
{
    ShadowCasting2D simulation;

    if(simulation.Construct(640, 480, 2, 2))
    {
        simulation.Start();
    }

}