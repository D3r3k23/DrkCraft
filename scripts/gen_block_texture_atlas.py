from typing import *
import argparse
import sys
import os.path
import dataclasses
import enum
import math

import PIL
from ruamel.yaml import YAML
yaml = YAML(typ='safe')

@dataclasses.dataclass
class Vec2:
    x: int = 0
    y: int = 0

Texture = NewType(PIL.Image.Image)
Box = NewType(tuple[int, int] | tuple[int, int, int, int])

def make_box(upperleft: Vec2) -> Box:
    return (upperleft.x, upperleft.y)

def make_box(upperleft: Vec2, bottomright: Vec2) -> Box:
    return (upperleft.x, upperleft.y, bottomright.x, bottomright.y)

TEXTURE_MODE = 'RGBA'

BLOCK_TEXTURE_SIZE = 16
ATLAS_MAX_WIDTH    = 16

NUM_TEXTURES_PER_BLOCK = 3

def main() -> Optional[int]:
    parser = argparse.ArgumentParser(description='Generates textures atlas image for all blocks')
    parser.add_argument(
        'atlas',
        type=str,
        nargs='?',
        default=os.path.join('assets', 'images', 'textures', 'block_atlas.png'),
        description='Output texture .png file'
    )
    parser.add_argument(
        '--blocks',
        type=str,
        default=os.path.join('data', 'blocks.yaml'),
        description='blocks.yaml file'
    )
    parser.add_argument(
        '--textures',
        type=str,
        default=os.path.join('assets', 'images', 'textures', 'blocks'),
        description='Block textures directory'
    )
    args = parser.parse_args()

    class PathType(enum.Enum):
        File = 0
        Dir = 1

    files: list[tuple[str, str, PathType]] = [
        ( args.atlas, 'texture atlas', PathType.File ),
        ( args.blocks, 'blocks YAML', PathType.File ),
        ( args.textures, 'textures', PathType.Dir )
    ]
    for (path, description, type) in files:
        if not (os.path.isfile(path) if type == PathType.File else os.path.isdir(path)):
            print(f'Error: {description} {type} "{path}" not found')
            return 1

    gen_block_texture_atlas(args.blocks, args.textures, args.atlas)

def gen_block_texture_atlas(blocks_yaml_fn: str, texture_dir: str, atlas_name: str):
    print(f'Loading "{blocks_yaml_fn}"')
    blocks = load_yaml(blocks_yaml_fn)
    if blocks is None:
        print(f'Error: Could not open blocks YAML: "{blocks_yaml_fn}"')
        return

    print('Loading textures')
    texture_names = blocks.keys()
    textures = load_textures(texture_dir, texture_names, '.png')
    print(f'{len(textures)} textures loaded')

    print('Generating texture atlas')
    atlas = create_block_texture_atlas_image(texture_dir, textures)

    print(f'Saving texture atlas to "{atlas_name}"')
    atlas.save(os.path.join(texture_dir, atlas_name))

def load_textures(texture_dir: str, texture_names: list[str], f_ext: str) -> list[Texture]:
    texturefiles = [ os.path.join(texture_dir, texture) + f_ext for texture in texture_names ]
    textures: list[Texture] = []
    for filename in texturefiles:
        try:
            f = open(filename, 'rb')
        except (OSError, IOError):
            print(f'Error: could not open texture file "{filename}"')
        else:
            try:
                image = PIL.Image.open(f)
            except PIL.UnidentifiedImageError:
                print(f'Error: missing texture file: "{filename}"')
            else:
                blocktextures = get_block_textures(image)
                textures.append(blocktextures)
            f.close()
    return textures

def get_block_textures(image: Texture) -> list[Texture]:
    def get_block_face_box(faceindex: int) -> Box:
        upperleft   = Vec2(BLOCK_TEXTURE_SIZE * faceindex, 0)
        bottomright = Vec2(upperleft.x + BLOCK_TEXTURE_SIZE, upperleft.y + BLOCK_TEXTURE_SIZE)
        return make_box(upperleft, bottomright)

    return [ image.crop(get_block_face_box(i)) for i in range(NUM_TEXTURES_PER_BLOCK) ]

def create_block_texture_atlas_image(textures: list[Texture]) -> Texture:
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

def find_texture_atlas_size(textures: list[Texture]) -> Vec2:
    count = len(textures)
    return Vec2(max(count, ATLAS_MAX_WIDTH), int(math.ceil(count / ATLAS_MAX_WIDTH)))

def load_yaml(filename: str) -> Optional[Mapping]:
    try:
        with open(filename, 'r') as f:
            return yaml.load(f)
    except (OSError, IOError):
        return None

if __name__ == '__main__':
    sys.exit(main())
