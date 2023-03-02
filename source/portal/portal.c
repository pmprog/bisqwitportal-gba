
#include "portal.h"
#include "portal_entity.h"

#define MAX_PORTAL_RENDERS  32

static portal_map* active_map;
static float hfov;
static float vfov;
static u8* renderedsectors = NULL;
static vec2 ywindow[240];  // x = top, y = bottom

portal_entity active_entities[255];
u8 active_entity_count;

void portal_initialise(portal_map* Map, float HorzFoV, float VertFoV)
{
    active_map = Map;
    // Clear out entities
    memset((void*)active_entities, 0, sizeof(active_entities));
    // Copy map defaults
    memcpy((void*)active_entities, Map->entity_list, sizeof(portal_entity) * Map->entity_count);
    active_entity_count = Map->entity_count;

    // Allow FoV to be configured
    hfov = HorzFoV;
    vfov = VertFoV;

    // Create render history
    renderedsectors = (u8*)calloc( Map->sector_count * sizeof(u8), 1 );
}

void portal_entity_add(portal_entity* NewEntity)
{

}

void portal_update()
{

    // Process Entities
    // portal_processentities();

}

void portal_render(portal_entity* Origin, u8 RenderWidth)
{
    portal_renderitem queue[MAX_PORTAL_RENDERS];
    u8 head = 1;
    u8 tail = 0;
    u8 now = 0;
    u16 neighbour = 0xFFFF;
    float pcos = fixed_to_float(cosi(Origin->rotation));
    float psin = fixed_to_float(sini(Origin->rotation));

    // Set default window to full height
    for( u8 i = 0; i < 240; ++i )
    {
        ywindow[i].x = 0;
        ywindow[i].y = 159;
    }

    // Clear render queue
    memset( renderedsectors, 0, active_map->sector_count * sizeof(u8) );

    // Start in player's sector
    queue[0].sector = Origin->position_sector;
    queue[0].sx1 = 0;
    queue[0].sx2 = RenderWidth - 1;

    do
    {

        // Start at the "end" of the list, maybe head and tail names should be reversed?
        now = tail;
        if( ++tail >= MAX_PORTAL_RENDERS ) { tail = 0; }

        if( renderedsectors[queue[now].sector] > MAX_PORTAL_RENDERS || (renderedsectors[queue[now].sector] & 0x01) ) { continue; }

        ++renderedsectors[queue[now].sector];
        portal_sector* sect = &active_map->sector_list[queue[now].sector];

        for(u8 s = 0; s < sect->vertices; ++s )
        {
            // Get the corners of the sector edge
            u8 n = (s + 1 >= sect->vertices ? s + 1 - sect->vertices : s + 1);
            portal_vec2 v1 = { sect->vertex_list[s].x - Origin->position.x, sect->vertex_list[s].y - Origin->position.y };
            portal_vec2 v2 = { sect->vertex_list[n].x - Origin->position.x, sect->vertex_list[n].y - Origin->position.y };

            // Rotate them around the Origin
            portal_vec2 t1 = { (v1.x * psin) - (v1.y * pcos), (v1.x * pcos) + (v1.y * psin) };
            portal_vec2 t2 = { (v2.x * psin) - (v2.y * pcos), (v2.x * pcos) + (v2.y * psin) };

            // Is wall partially in front of the camera?
            if( t1.y <= 0.0f && t2.y <= 0.0f ) { continue; }
            vec2 lineintersect;
            if (t1.y < 0.0f)
            {
                if (float_linesintersect(t1.x, t1.y, t2.x, t2.y, 0, 0, -1000.0f, 0, &lineintersect) != 0)
                {
                    if (t1.y <= 0.0f) { t1.x = lineintersect.x; t1.y = lineintersect.y; }
                    if (t2.y <= 0.0f) { t2.x = lineintersect.x; t2.y = lineintersect.y; }
                }
            }
            if (t2.y < 0.0f)
            {
                if (float_linesintersect(t1.x, t1.y, t2.x, t2.y, 0, 0, 1000.0f, 0, &lineintersect) != 0)
                {
                    if (t1.y <= 0.0f) { t1.x = lineintersect.x; t1.y = lineintersect.y; }
                    if (t2.y <= 0.0f) { t2.x = lineintersect.x; t2.y = lineintersect.y; }
                }
            }

            neighbour = sect->edge_sector_list[s];
            vec2 yneighbour = { 0, 0 };  // x = top, y = bottom
            graphics_bitmap_line(80 - (s32)(t1.x), 80 - (s32)(t1.y), 80 - (s32)(t2.x), 80 - (s32)(t2.y), (neighbour != 0xFFFF ? 9 : 2));

            /// Do perspective transformation
            float xscale1 = (hfov / t1.y);
            float yscale1 = (vfov / t1.y);
            s16 x1 = (s16)((RenderWidth >> 1) - (s32)(t1.x * xscale1));
            float xscale2 = (hfov / t2.y);
            float yscale2 = (vfov / t2.y);
            s16 x2 = (s16)((RenderWidth >> 1) - (s32)(t2.x * xscale2));

            // Only render if the wall is visible
            if( x1 >= x2 || x2 < queue[now].sx1 || x1 > queue[now].sx2 ) { continue; }

            // Adjust floor/ceiling based on Origin
            float yceil = sect->ceiling - Origin->position.z;
            float yfloor = sect->floor - Origin->position.z;

            // Does the wall connect to another sector?
            if( neighbour != 0xFFFF )
            {
                yneighbour.x = active_map->sector_list[neighbour].ceiling - Origin->position.z;
                yneighbour.y = active_map->sector_list[neighbour].floor - Origin->position.z;
            }

            // Project floor and ceiling into the screen
            s32 y1a = 80 - (s32)(yceil * yscale1);
            s32 y1b = 80 - (s32)(yfloor * yscale1);
            s32 y2a = 80 - (s32)(yceil * yscale2);
            s32 y2b = 80 - (s32)(yfloor * yscale2);

            // Draw the wall
            s16 beginx = max(x1, queue[now].sx1);
            s16 endx = min(x2, queue[now].sx2);
            for(s16 x = beginx; x <= endx; ++x)
            {
                // Calculation for lighting levels
                // int z = ((x - x1) * (tz2 - tz1) / (x2 - x1) + tz1) * 8;

                // Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them.
                s16 ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
                //s16 ya = (s32)(float_divide(int_to_float((x - x1) * (y2a - y1a)), int_to_float((x2 - x1) + y1a)));
                s16 cya = maths_clamp(ya, ywindow[x].x, ywindow[x].y); // top
                s16 yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;
                //s16 yb = (s32)(float_divide(int_to_float((x - x1) * (y2b - y1b)), int_to_float((x2 - x1) + y1b)));
                s16 cyb = maths_clamp(yb, ywindow[x].x, ywindow[x].y); // bottom

                // Render to ceiling
                graphics_bitmap_line(x, ywindow[x].x, x, cya, 3);
                // Render the floor
                graphics_bitmap_line(x, cyb, x, ywindow[x].y, 5);

                if (neighbour != 0xFFFF)
                {
                    // The same for the neighboring sector
                    s32 ny1a = 80 - (s32)(yneighbour.x * yscale1);
                    s32 ny1b = 80 - (s32)(yneighbour.y * yscale1);
                    s32 ny2a = 80 - (s32)(yneighbour.x * yscale2);
                    s32 ny2b = 80 - (s32)(yneighbour.y * yscale2);

                    // Same for _their_ floor and ceiling 
                    s16 nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a;
                    s16 cnya = maths_clamp(nya, ywindow[x].x, ywindow[x].y);
                    s16 nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b;
                    s16 cnyb = maths_clamp(nyb, ywindow[x].x, ywindow[x].y);
                    // If our ceiling is higher than their ceiling, render upper wall
                    graphics_bitmap_line(x, cya, x, cnya - 1, sect->edge_texture_list[s]); // Between our and their ceiling
                    ywindow[x].x = maths_clamp(maths_max(cya, cnya), ywindow[x].x, 159);   // Shrink the remaining window below these ceilings
                    // If our floor is lower than their floor, render bottom wall
                    graphics_bitmap_line(x, cnyb + 1, x, cyb, sect->edge_texture_list[s]); // Between their and our floor
                    ywindow[x].y = maths_clamp(maths_min(cyb, cnyb), 0, ywindow[x].y); // Shrink the remaining window above these floors
                }
                else
                {
                    // Render Wall
                    graphics_bitmap_line(x, cya, x, cyb, sect->edge_texture_list[s]);
                    graphics_bitmap_setpixel(x, cya, 1);
                    graphics_bitmap_setpixel(x, cyb, 1);
                }

            }

            // Schedule the neighboring sector for rendering within the window formed by this wall.
            if (neighbour != 0xFFFF && endx >= beginx && (head + MAX_PORTAL_RENDERS + 1 - tail) % MAX_PORTAL_RENDERS)
            {
                queue[head].sector = neighbour;
                queue[head].sx1 = beginx;
                queue[head].sx2 = endx;
                if( ++head >= MAX_PORTAL_RENDERS ) { head = 0; };
            }

        }
        ++renderedsectors[queue[now].sector];
    } while( head != tail );

}
