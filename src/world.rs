// world.rs

use std::collections::HashMap;
use macroquad::prelude::*;

pub const CHUNK_SIZE: usize = 16;
pub const CHUNK_VOLUME: usize = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

pub struct VoxelData {
    texture: Texture2D
}

impl VoxelData {
    pub fn new(texture: Texture2D) -> Self { Self { texture } }

    pub async fn from_file(filename: &str) -> Result<VoxelData, FileError> {
        match load_texture(filename).await {
            Ok(texture) => {
                texture.set_filter(FilterMode::Nearest);
                Ok(Self::new(texture))
            },
            Err(err) => Err(err),
        }
    }
}


#[derive(Copy, Clone)]
pub struct Voxel {
    id: usize
}

impl Voxel {
    pub fn new(id: usize) -> Self { Self { id } }

    fn render(self: &Self, position: IVec3, voxel_data: &Vec<VoxelData>) {
        draw_cube(
            position.as_vec3(), vec3(1.0, 1.0, 1.0),
            voxel_data.get(self.id).unwrap().texture, WHITE
        );
    }
}

struct Chunk {
    voxels: [Option<Voxel>; CHUNK_VOLUME]
}

impl Chunk {
    fn new(voxels: [Option<Voxel>; CHUNK_VOLUME]) -> Self { Self { voxels } }

    fn empty() -> Self { Self::new([None; CHUNK_VOLUME]) }

    // TODO: voxels should be built into one big mesh to be rendered here,
    // rather than rendering each voxel as its own cube in Voxel::render()
    fn render(self: &Self, chunk_position: IVec3, registry: &Vec<VoxelData>) {
        let size: i32 = CHUNK_SIZE.try_into().unwrap();
        let mut index: i32 = 0;

        for voxel in &self.voxels {
            if let Some(voxel) = voxel {
                let voxel_position = ivec3(
                    index % size,
                    (index / size) % size,
                    index / (size * size)
                );
                voxel.render(chunk_position + voxel_position, registry);
            }
            index += 1;
        }
    }
}

pub struct World {
    chunks: HashMap<IVec3, Chunk>
}

impl World {
    fn new(chunks: HashMap<IVec3, Chunk>) -> Self { Self { chunks } }

    pub fn empty() -> Self { Self::new(HashMap::new()) }

    // TODO: temporary function; replace with something better
    pub fn add_voxel(
        self: &mut Self,
        world_position: IVec3,
        chunk_position: usize,
        voxel: Voxel
    ) {
        let mut chunk = match self.chunks.get_mut(&world_position) {
            Some(chunk) => chunk,
            None => {
                self.chunks.insert(world_position, Chunk::empty());
                self.chunks.get_mut(&world_position).unwrap()
            },
        };
        chunk.voxels[chunk_position] = Some(voxel);
    }

    pub fn render(self: &Self, registry: &Vec<VoxelData>) {
        for (position, chunk) in &self.chunks {
            chunk.render(*position, registry);
        }
    }
}
