import PIL
from ruamel.yaml import YAML
yaml = YAML(typ='safe')

from typing import *
from dataclasses import dataclass
import argparse
import os.path
import math

@dataclass
class Vec2:
    x: int = 0
    y: int = 0

Texture = NewType(PIL.Image.Image)
Box = NewType(Tuple[int, int] | Tuple[int, int, int, int])

def make_box(upperleft: Vec2) -> Box:
    return (upperleft.x, upperleft.y)

def make_box(upperleft: Vec2, bottomright: Vec2) -> Box:
    return (upperleft.x, upperleft.y, bottomright.x, bottomright.y)

TEXTURE_PATH = os.path.join('assets', 'images', 'textures')
TEXTURE_EXT  = '.png'
TEXTURE_MODE = 'RGBA'

BLOCK_TEXTURE_SIZE = 16
ATLAS_MAX_WIDTH    = 16

NUM_TEXTURES_PER_BLOCK = 3

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('blocks_yaml', type=str, default=os.path.join('data', 'blocks.yaml'))
    parser.add_argument('atlas',       type=str, default='blockatlas.png')
    args = parser.parse_args()

    gen_block_texture_atlas(args.blocks_yaml, args.atlas)

def gen_block_texture_atlas(blocksyaml_fn: str, atlasname: str):
    print(f'Loading "{blocksyaml_fn}"')
    blocks = load_yaml(blocksyaml_fn)
    if blocks is None:
        print(f'Error: Could not open blocks YAML "{blocksyaml_fn}"')
        return

    print('Loading textures')
    blocktextures = [ get_block_texture_filename(block) for block in blocks.keys() ]
    textures = load_textures(blocktextures)
    print(f'{len(textures)} textures loaded')

    print('Generating texture atlas')
    atlas = create_block_texture_atlas_image(textures)

    print(f'Saving texture atlas to "{atlasname}"')
    atlas.save(os.path.join(TEXTURE_PATH, atlasname))

def create_block_texture_atlas_image(textures: List[Texture]) -> Texture:
    size = find_texture_atlas_size(textures)
    atlas = PIL.Image.new(TEXTURE_MODE, (size.x * BLOCK_TEXTURE_SIZE, size.y * BLOCK_TEXTURE_SIZE))

    def get_atlas_texture_box(atlasindex: int) -> Box:
        coord       = Vec2(atlasindex % ATLAS_MAX_WIDTH, atlasindex // ATLAS_MAX_WIDTH)
        upperleft   = Vec2(coord.x * BLOCK_TEXTURE_SIZE, coord.y * BLOCK_TEXTURE_SIZE)
        bottomright = Vec2(upperleft.x1 + BLOCK_TEXTURE_SIZE, upperleft.y1 + BLOCK_TEXTURE_SIZE)
        return make_box(upperleft, bottomright)

    print('Compositing atlas')
    for i, texture in enumerate(textures):
        atlas.paste(texture, get_atlas_texture_box(i))

    return atlas

def load_textures(texturefiles: List[str]) -> List[Texture]:
    textures: List[Texture] = []
    for filename in texturefiles:
        try:
            f = open(filename, 'rb')
        except (OSError, IOError):
            print(f'Error: could not open texture file "{filename}"')
        else:
            try:
                image = PIL.Image.open(f)
            except PIL.UnidentifiedImageError:
                print(f'Error: invalid image file "{filename}"')
            else:
                blocktextures = get_block_textures(image)
                textures.append(blocktextures)
            f.close()
    return textures

def get_block_texture_filename(blockname: str) -> str:
    return os.path.join(TEXTURE_PATH, blockname + TEXTURE_EXT)

def get_block_textures(image: Texture) -> List[Texture]:
    def get_block_face_box(faceindex: int) -> Box:
        upperleft   = Vec2(BLOCK_TEXTURE_SIZE * faceindex, 0)
        bottomright = Vec2(upperleft.x + BLOCK_TEXTURE_SIZE, upperleft.y + BLOCK_TEXTURE_SIZE)
        return make_box(upperleft, bottomright)

    return [ image.crop(get_block_face_box(i)) for i in range(NUM_TEXTURES_PER_BLOCK) ]

def find_texture_atlas_size(textures: List[Texture]) -> Vec2:
    count = len(textures)
    return Vec2(max(count, ATLAS_MAX_WIDTH), int(math.ceil(count / ATLAS_MAX_WIDTH)))

def load_yaml(filename: str) -> Optional[yaml]:
    try:
        with open(filename, 'r') as f:
            return yaml.load(f)
    except (OSError, IOError):
        return None

if __name__ == '__main__':
    main()
