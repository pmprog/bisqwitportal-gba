
#include "stage_bitmaptest.h"
#include <math.h>

#define NumSectors 2

const xy sector0_vertex[] = { { 0, 0}, {12, 0}, {12, 12}, {0, 12 }, { 0, 0} };
u8 sector0_neighbors[] = { -1, 1, -1, -1 };

const xy sector1_vertex[] = { { 12, 0}, {24, 0}, {24, 24}, {12, 12 }, { 12, 0 } };
u8 sector1_neighbors[] = { -1, -1, -1, 0 };

const sector sectors[] = {
    { 0, 20, sector0_vertex, sector0_neighbors, 5 },
    { 2, 25, sector1_vertex, sector1_neighbors, 5 },
};

u8 renderedsectors[] = { 0, 0 };
static playerinfo player = { {2, 6, 0}, {0,0,0}, 0,0,0,0, 0 };
static u8 moving = 0;
static u8 ground = 0;
static u8 falling = 1;


static void vline(int x, int y1, int y2, int top, int middle, int bottom);
static void MovePlayer(float dx, float dy);
static void DrawScreen();

void bitmaptest_start()
{
    graphics_mode_bitmap( 0 );
#ifdef LIBGBA
    REG_BG2PA = 0x80;   // Stretch
#endif
    MovePlayer(0, 0);
}

void bitmaptest_finish()
{

}

void bitmaptest_update()
{
    ground = !falling;
    if (falling)
    {
        player.velocity.z -= 0.05f; /* Add gravity */
        float nextz = player.where.z + player.velocity.z;
        if (player.velocity.z < 0 && nextz < sectors[player.sector].floor + EyeHeight) // When going down
        {
            /* Fix to ground */
            player.where.z = sectors[player.sector].floor + EyeHeight;
            player.velocity.z = 0;
            falling = 0;
            ground = 1;
        }
        else if (player.velocity.z > 0 && nextz > sectors[player.sector].ceil) // When going up
        {
            /* Prevent jumping above ceiling */
            player.velocity.z = 0;
            falling = 1;
        }
        if (falling)
        {
            player.where.z += player.velocity.z;
            moving = 1;
        }
    }
    if (moving)
    {
        float px = player.where.x, py = player.where.y;
        float dx = player.velocity.x, dy = player.velocity.y;

        const struct sector* const sect = &sectors[player.sector];
        const struct xy* const vert = sect->vertex;
        /* Check if the player is about to cross one of the sector's edges */
        for (unsigned s = 0; s < sect->npoints; ++s)
            if (IntersectBox(px, py, px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y)
                && PointSide(px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) < 0)
            {
                /* Check where the hole is. */
                float hole_low = sect->neighbors[s] < 0 ? 9e9 : max(sect->floor, sectors[sect->neighbors[s]].floor);
                float hole_high = sect->neighbors[s] < 0 ? -9e9 : min(sect->ceil, sectors[sect->neighbors[s]].ceil);
                /* Check whether we're bumping into a wall. */
                if (hole_high < player.where.z + HeadMargin
                    || hole_low  > player.where.z - EyeHeight + KneeHeight)
                {
                    /* Bumps into a wall! Slide along the wall. */
                    /* This formula is from Wikipedia article "vector projection". */
                    float xd = vert[s + 1].x - vert[s + 0].x, yd = vert[s + 1].y - vert[s + 0].y;
                    dx = xd * (dx * xd + yd * dy) / (xd * xd + yd * yd);
                    dy = yd * (dx * xd + yd * dy) / (xd * xd + yd * yd);
                    moving = 0;
                }
            }
        MovePlayer(dx, dy);
    }

    DrawScreen();

    float move_vec[2] = { 0.f, 0.f };
    u8 pushing = 0;
    if (input_button_held(DPAD_Up)) 
    {
        move_vec[0] += player.anglecos * 0.2f; 
        move_vec[1] += player.anglesin * 0.2f; 
        pushing = 1;
    }
    if (input_button_held(DPAD_Down)) 
    {
        move_vec[0] -= player.anglecos * 0.2f; 
        move_vec[1] -= player.anglesin * 0.2f; 
        pushing = 1;
    }
    if (input_button_held(BUTTON_L)) 
    { 
        move_vec[0] += player.anglesin * 0.2f; 
        move_vec[1] -= player.anglecos * 0.2f; 
        pushing = 1;
    }
    if (input_button_held(BUTTON_R)) 
    { 
        move_vec[0] -= player.anglesin * 0.2f; 
        move_vec[1] += player.anglecos * 0.2f; 
        pushing = 1;
    }
    if (input_button_held(DPAD_Left))
    {
        player.angle -= 0.2f;
        //yaw = clamp(yaw - y * 0.05f, -5, 5);
        //player.yaw = yaw - player.velocity.z * 0.5f;
        MovePlayer(0, 0);
    }
    if (input_button_held(DPAD_Right))
    {
        player.angle += 0.2f;
        //yaw = clamp(yaw - y * 0.05f, -5, 5);
        //player.yaw = yaw - player.velocity.z * 0.5f;
        MovePlayer(0, 0);
    }
    float acceleration = pushing ? 0.4 : 0.2;

    player.velocity.x = player.velocity.x * (1 - acceleration) + move_vec[0] * acceleration;
    player.velocity.y = player.velocity.y * (1 - acceleration) + move_vec[1] * acceleration;

    if (pushing) moving = 1;

}



/* vline: Draw a vertical line on screen, with a different color pixel in top & bottom */
IWRAM_CODE static void vline(int x, int y1, int y2, int top, int middle, int bottom)
{
    y1 = clamp(y1, 0, H - 1);
    y2 = clamp(y2, 0, H - 1);
    if (y2 == y1)
    {
        graphics_bitmap_setpixel(x, y1, middle);
    }
    else if (y2 > y1)
    {
        //graphics_bitmap_setpixel(x, y1, top);
        graphics_bitmap_line(x, y1 + 1, x, y2 - 1, middle);
        //graphics_bitmap_setpixel(x, y2, bottom);
    }
}

/* MovePlayer(dx,dy): Moves the player by (dx,dy) in the map, and
 * also updates their anglesin/anglecos/sector properties properly.
 */
static void MovePlayer(float dx, float dy)
{
    float px = player.where.x, py = player.where.y;
    /* Check if this movement crosses one of this sector's edges
     * that have a neighboring sector on the other side.
     * Because the edge vertices of each sector are defined in
     * clockwise order, PointSide will always return -1 for a point
     * that is outside the sector and 0 or 1 for a point that is inside.
     */
    const struct sector* const sect = &sectors[player.sector];
    const struct xy* const vert = sect->vertex;
    for (unsigned s = 0; s < sect->npoints; ++s)
        if (sect->neighbors[s] >= 0
            && IntersectBox(px, py, px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y)
            && PointSide(px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) < 0)
        {
            player.sector = sect->neighbors[s];
            break;
        }

    player.where.x += dx;
    player.where.y += dy;
    player.anglesin = sin(player.angle);
    player.anglecos = cos(player.angle);
}

static void DrawScreen()
{
    enum { MaxQueue = 32 };  // maximum number of pending portal renders
    struct item { int sectorno, sx1, sx2; } queue[MaxQueue], * head = queue, * tail = queue;
    int ytop[W] = { 0 };
    int ybottom[W];

    for (u32 x = 0; x < W; ++x)
    {
        ybottom[x] = H - 1;
    }
    
    memset(renderedsectors, 0, NumSectors);

    /* Begin whole-screen rendering from where the player is. */
    *head = (struct item){ player.sector, 0, W - 1 };
    if (++head == queue + MaxQueue) head = queue;

    do {
        /* Pick a sector & slice from the queue to draw */
        const struct item now = *tail;
        if (++tail == queue + MaxQueue) tail = queue;

        if (renderedsectors[now.sectorno] & 0x21) continue; // Odd = still rendering, 0x20 = give up
        ++renderedsectors[now.sectorno];
        const struct sector* const sect = &sectors[now.sectorno];
        /* Render each wall of this sector that is facing towards player. */
        for (unsigned s = 0; s < sect->npoints; ++s)
        {
            /* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
            float vx1 = sect->vertex[s + 0].x - player.where.x, vy1 = sect->vertex[s + 0].y - player.where.y;
            float vx2 = sect->vertex[s + 1].x - player.where.x, vy2 = sect->vertex[s + 1].y - player.where.y;
            /* Rotate them around the player's view */
            float pcos = player.anglecos, psin = player.anglesin;
            float tx1 = vx1 * psin - vy1 * pcos, tz1 = vx1 * pcos + vy1 * psin;
            float tx2 = vx2 * psin - vy2 * pcos, tz2 = vx2 * pcos + vy2 * psin;
            /* Is the wall at least partially in front of the player? */
            if (tz1 <= 0 && tz2 <= 0) continue;
            /* If it's partially behind the player, clip it against player's view frustrum */
            if (tz1 <= 0 || tz2 <= 0)
            {
                float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
                // Find an intersection between the wall and the approximate edges of player's view
                struct xy i1 = Intersect(tx1, tz1, tx2, tz2, -nearside, nearz, -farside, farz);
                struct xy i2 = Intersect(tx1, tz1, tx2, tz2, nearside, nearz, farside, farz);
                if (tz1 < nearz) { if (i1.y > 0) { tx1 = i1.x; tz1 = i1.y; } else { tx1 = i2.x; tz1 = i2.y; } }
                if (tz2 < nearz) { if (i1.y > 0) { tx2 = i1.x; tz2 = i1.y; } else { tx2 = i2.x; tz2 = i2.y; } }
            }
            /* Do perspective transformation */
            float xscale1 = hfov / tz1, yscale1 = vfov / tz1;    int x1 = W / 2 - (int)(tx1 * xscale1);
            float xscale2 = hfov / tz2, yscale2 = vfov / tz2;    int x2 = W / 2 - (int)(tx2 * xscale2);
            if (x1 >= x2 || x2 < now.sx1 || x1 > now.sx2) continue; // Only render if it's visible
            /* Acquire the floor and ceiling heights, relative to where the player's view is */
            float yceil = sect->ceil - player.where.z;
            float yfloor = sect->floor - player.where.z;
            /* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
            int neighbor = sect->neighbors[s];
            float nyceil = 0, nyfloor = 0;
            if (neighbor >= 0) // Is another sector showing through this portal?
            {
                nyceil = sectors[neighbor].ceil - player.where.z;
                nyfloor = sectors[neighbor].floor - player.where.z;
            }
            /* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
#define Yaw(y,z) (y + z*player.yaw)
            int y1a = H / 2 - (int)(Yaw(yceil, tz1) * yscale1), y1b = H / 2 - (int)(Yaw(yfloor, tz1) * yscale1);
            int y2a = H / 2 - (int)(Yaw(yceil, tz2) * yscale2), y2b = H / 2 - (int)(Yaw(yfloor, tz2) * yscale2);
            /* The same for the neighboring sector */
            int ny1a = H / 2 - (int)(Yaw(nyceil, tz1) * yscale1), ny1b = H / 2 - (int)(Yaw(nyfloor, tz1) * yscale1);
            int ny2a = H / 2 - (int)(Yaw(nyceil, tz2) * yscale2), ny2b = H / 2 - (int)(Yaw(nyfloor, tz2) * yscale2);

            /* Render the wall. */
            int beginx = max(x1, now.sx1), endx = min(x2, now.sx2);
            for (int x = beginx; x <= endx; ++x)
            {
                /* Calculate the Z coordinate for this point. (Only used for lighting.) */
                int z = ((x - x1) * (tz2 - tz1) / (x2 - x1) + tz1) * 8;
                /* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
                int ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a, cya = clamp(ya, ytop[x], ybottom[x]); // top
                int yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b, cyb = clamp(yb, ytop[x], ybottom[x]); // bottom

                /* Render ceiling: everything above this sector's ceiling height. */
                vline(x, ytop[x], cya - 1, 3, 4, 3);
                /* Render floor: everything below this sector's floor height. */
                vline(x, cyb + 1, ybottom[x], 5, 6, 5);

                /* Is there another sector behind this edge? */
                if (neighbor >= 0)
                {
                    /* Same for _their_ floor and ceiling */
                    int nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a, cnya = clamp(nya, ytop[x], ybottom[x]);
                    int nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b, cnyb = clamp(nyb, ytop[x], ybottom[x]);
                    /* If our ceiling is higher than their ceiling, render upper wall */
                    unsigned r1 = 12, r2 = 13;  // 0x010101 * (255 - z), r2 = 0x040007 * (31 - z / 8);
                    vline(x, cya, cnya - 1, 1, x == x1 || x == x2 ? 1 : r1, 1); // Between our and their ceiling
                    ytop[x] = clamp(max(cya, cnya), ytop[x], H - 1);   // Shrink the remaining window below these ceilings
                    /* If our floor is lower than their floor, render bottom wall */
                    vline(x, cnyb + 1, cyb, 1, x == x1 || x == x2 ? 1 : r2, 1); // Between their and our floor
                    ybottom[x] = clamp(min(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
                }
                else
                {
                    /* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
                    unsigned r = 15; // 0x010101 * (255 - z);
                    vline(x, cya, cyb, 1, x == x1 || x == x2 ? 1 : r, 1);
                }
            }
            /* Schedule the neighboring sector for rendering within the window formed by this wall. */
            if (neighbor >= 0 && endx >= beginx && (head + MaxQueue + 1 - tail) % MaxQueue)
            {
                *head = (struct item){ neighbor, beginx, endx };
                if (++head == queue + MaxQueue) head = queue;
            }
        } // for s in sector's edges
        ++renderedsectors[now.sectorno];
    } while (head != tail); // render any other queued sectors
}

