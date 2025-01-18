"""
Using bash to count lines was a bit annoying so I wrote this script to do it better.
"""

from os import path
import glob
from pathlib import Path


def main() -> None:
    """
    This is the main function that runs the line counter.
    :return: None
    """
    print("Starting line counter...")

    # first use code from linguist_vendorer.py to get the banned directories
    path_to_src: str = '../src'

    # put all vendored folders here. These must be relative to the src directory.
    misc_vendored_folders: list[str] = ['lib/Noise',
                                        'lib/Adafruit_NeoPixel']
    misc_vendored_folders = [path_to_src + '/' + folder for folder in misc_vendored_folders]

    # find all directories inside to /src. Glob is a recursive search so this will
    # find all directories.
    all_directory_paths: list[str] = glob.glob(path_to_src + '/**/', recursive=True)

    # put all banned directories here. These must be relative to the src directory.
    banned_directory_paths: list[str] = ['Libs/']

    # Go through all the directories and find if any are a child (or lower relative)
    # to a banned directory.
    all_banned_paths: list[str] = []
    for directory in all_directory_paths:

        # figure out if path is inside a banned directory
        child_path = Path(path.abspath(directory))
        banned = False
        for banned_dir in banned_directory_paths:
            banned_dir = path_to_src + '/' + banned_dir
            if child_path.is_relative_to(path.abspath(banned_dir)):
                banned = True
                break
        if banned:  # if it is then skip this round of the for loop
            all_banned_paths.append(directory)
            continue

    all_banned_paths += misc_vendored_folders  # add the extra banned folders to the banned paths

    # convert to absolute paths
    all_banned_paths = [path.abspath(banned_path) for banned_path in all_banned_paths]
    all_directory_paths = [path.abspath(directory) for directory in all_directory_paths]

    # get all allowed paths
    all_allowed_paths = [directory for directory in all_directory_paths
                         if directory not in all_banned_paths]

    # get all file paths
    all_file_paths: list[str] = []
    for directory in all_allowed_paths:
        all_file_paths += glob.glob(directory + '**/*.h')
        all_file_paths += glob.glob(directory + '**/*.cpp')

    print(f'There are {len(all_file_paths)} files.')

    # count all lines
    total_lines: int = 0
    for file in all_file_paths:
        with open(file, 'r', encoding='utf-8') as opened_file:
            total_lines += len(opened_file.readlines())

    print(f'Total lines: {total_lines}')

    print("Line counter complete!")


if __name__ == '__main__':
    main()