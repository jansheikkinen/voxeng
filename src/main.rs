// main.rs

#![allow(dead_code)]

mod world;
use world::*;

use macroquad::prelude::*;

struct Game {
    worlds: Vec<World>,
    voxels: Vec<VoxelData>,
}

impl Game {
    fn render(self: &Self) {
        for world in &self.worlds {
            world.render(&self.voxels);
        }
    }
}

#[macroquad::main("voxeng")]
async fn main() {
    let mut game = Game {
        worlds: vec![World::empty()],
        voxels: vec![VoxelData::from_file("texture.png").await.unwrap()]
    };

    let origin = IVec3::new(0, 0, 0);
    let mut i: usize = 0;

    loop {
        if i < CHUNK_VOLUME {
            game.worlds[0].add_voxel(origin, i, Voxel::new(0));
            i += 1;
        }

        clear_background(LIGHTGRAY);
        set_camera(&Camera3D {
            position: vec3(-30.0, 30.0, 30.0),
            up: vec3(0.0, 1.0, 0.0),
            target: vec3(0.0, 0.0, 0.0),
            ..Default::default()
        });
        draw_grid(20, 1.0, BLACK, GRAY);

        game.render();

        set_default_camera();
        draw_text(format!("FPS: {}", get_fps()).as_str(), 16.0, 32.0, 32.0, WHITE);

        next_frame().await
    }
}
