from typing import *
from pathlib import Path
from argparse import ArgumentParser
from dataclasses import dataclass
import math
import sys

import PIL
import PIL.Image
from PIL.Image import Image as Texture

from ruamel.yaml import YAML
yaml = YAML(typ='safe')

@dataclass
class Vec2:
    x: int = 0
    y: int = 0

@dataclass
class Box:
    upperleft: Vec2
    bottomright: Vec2

    @property
    def tup(self) -> tuple[int, int, int, int]:
        return ( self.upperleft.x, self.upperleft.y, self.bottomright.x, self.bottomright.y )

TEXTURE_MODE = 'RGBA'

BLOCK_TEXTURE_SIZE = 16
ATLAS_MAX_WIDTH    = 16

NUM_TEXTURES_PER_BLOCK = 3

def main(argv: list[str]=sys.argv) -> Optional[int]:
    prog, args = argv[0], argv[1:]

    description = 'Generates textures atlas image for all blocks'
    usage = f'{prog} [atlas] [--blocks BLOCKS] [--textures TEXTURES]'

    parser = ArgumentParser(prog=prog, description=description, usage=usage)
    parser.add_argument(
        'atlas', type=str, nargs='?', default='block_atlas.png',
        help='Output texture .png filename'
    )
    parser.add_argument(
        '--blocks', type=Path, default=Path('data/blocks.yaml'),
        help='blocks.yaml filename'
    )
    parser.add_argument(
        '--textures', type=Path, default=Path('assets/images/textures/blocks'),
        help='Block textures directory'
    )
    parsed_args = parser.parse_args(args)
    atlas:  str  = parsed_args.atlas
    blocks:  Path = parsed_args.blocks
    textures: Path = parsed_args.textures

    if not blocks.is_file():
        print(f'Error: Blocks YAML file not found: "{blocks}"')
        return 1
    if not textures.is_dir():
        print(f'Error: Textures dir not found: "{textures}"')
        return 1

    if not gen_block_texture_atlas(blocks, textures, atlas):
        return 1

def gen_block_texture_atlas(blocks_yaml_fn: Path, texture_dir: Path, atlas_name: str) -> bool:
    print(f'Loading "{blocks_yaml_fn}"')
    blocks = load_yaml(blocks_yaml_fn)
    if blocks is None:
        print(f'Error: Could not open blocks YAML: "{blocks_yaml_fn}"')
        return False

    print('Loading textures')
    texture_names = tuple(blocks.keys())
    textures = load_textures(texture_dir, texture_names)
    print(f'{len(textures)} textures loaded')

    print('Generating texture atlas')
    atlas = create_block_texture_atlas_image(textures)

    atlas_path = texture_dir / atlas_name
    print(f'Saving texture atlas to "{atlas_path}"')
    atlas.save(atlas_path)

    return True

def load_textures(texture_dir: Path, texture_names: Sequence[str], f_ext: str='.png') -> list[Texture]:
    texture_files = [ Path(texture_dir / texture).with_suffix(f_ext) for texture in texture_names ]
    textures: list[Texture] = []
    for filename in texture_files:
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
                block_textures = get_block_textures(image)
                textures.append(block_textures)
            f.close()
    return textures

def get_block_textures(image: Texture) -> list[Texture]:
    def get_block_face_box(face_index: int) -> Box:
        upperleft   = Vec2(BLOCK_TEXTURE_SIZE * face_index, 0)
        bottomright = Vec2(upperleft.x + BLOCK_TEXTURE_SIZE, upperleft.y + BLOCK_TEXTURE_SIZE)
        return Box(upperleft, bottomright)

    return [ image.crop(get_block_face_box(i).tup) for i in range(NUM_TEXTURES_PER_BLOCK) ]

def create_block_texture_atlas_image(textures: list[Texture]) -> Texture:
    size = find_texture_atlas_size(textures)
    atlas = PIL.Image.new(TEXTURE_MODE, (size.x * BLOCK_TEXTURE_SIZE, size.y * BLOCK_TEXTURE_SIZE))

    def get_atlas_texture_box(atlas_index: int) -> Box:
        coord       = Vec2(atlas_index % ATLAS_MAX_WIDTH, atlas_index // ATLAS_MAX_WIDTH)
        upperleft   = Vec2(coord.x * BLOCK_TEXTURE_SIZE, coord.y * BLOCK_TEXTURE_SIZE)
        bottomright = Vec2(upperleft.x + BLOCK_TEXTURE_SIZE, upperleft.y + BLOCK_TEXTURE_SIZE)
        return Box(upperleft, bottomright)

    print('Compositing atlas')
    for i, texture in enumerate(textures):
        atlas.paste(texture, get_atlas_texture_box(i).tup)

    return atlas

def find_texture_atlas_size(textures: list[Texture]) -> Vec2:
    count = len(textures)
    return Vec2(max(count, ATLAS_MAX_WIDTH), int(math.ceil(count / ATLAS_MAX_WIDTH)))

def load_yaml(filename: Path) -> Optional[Mapping]:
    try:
        with open(filename, 'r') as f:
            return yaml.load(f)
    except (OSError, IOError):
        return None

if __name__ == '__main__':
    sys.exit(main(sys.argv))
