**Languages:** English | [Español](readme.es.md) | [Français](readme.fr.md) | [日本語](readme.ja.md)

<p align="center">
  <img src="media/balatro.png" width="450" alt="Balatro">
</p>

**An unofficial homebrew port project aiming to bring Balatro to the Nintendo DS.**

Balatro DS is a personal, non-commercial development project created for learning, experimentation, and exploring the capabilities of Nintendo DS homebrew development.

The goal is to recreate the original PC version of _Balatro_ as faithfully as possible on Nintendo DS hardware, aiming for a 1:1 gameplay experience while adapting the game's systems, interface, and presentation to the limitations of the console.

This project is developed independently and is not affiliated with, endorsed by, or associated with the original developers or publishers of Balatro.

## Project Goals

- Recreate the core gameplay mechanics of the original PC version.
- Implement poker hand evaluation and scoring as faithfully as possible.
- Reproduce the original game's progression systems, rounds, and economy.
- Adapt the user interface and controls to the Nintendo DS.
- Explore the feasibility of a high-fidelity Balatro experience on original DS hardware.
- Learn and improve through low-level development and reverse engineering of game behavior.

## Current Status

**In active development — early playable prototype.**

The project is currently a basic, text-based prototype. The following features are implemented:

- Standard 52-card deck generation and shuffling.
- An eight-card starting hand.
- Card navigation and selection using the Nintendo DS controls.
- Basic poker hand evaluation.
- Initial chip, multiplier, and final score calculations.
- Basic card playing and discarding, including the ability to empty the hand.

The core gameplay loop is still incomplete. Current work is focused on building the foundations before moving on to the full round-based experience.


<p align="center">
  <img src="media/bal4.png" width="250" alt="Balatro">
  <img src="media/bal5.png" width="250" alt="Balatro">
  <img src="media/bal6.png" width="250" alt="Balatro">
</p>

### In Development / Planned

- Expanding the Joker system with more original effects.
- Randomized Joker shop offers, with three Jokers available per run.
- Complete round progression, blinds, and scoring targets.
- Hand and discard limits.
- Card replacement and improved deck management.
- Additional gameplay mechanics and card enhancements.
- Improved graphics, animations, and a more faithful interface adapted to the Nintendo DS dual screens.
- Further optimization for Nintendo DS hardware.

This is an early work in progress; features and compatibility may change as development continues.

## Development

The project is written in C using [BlocksDS](https://blocksds.skylyrac.net/), targeting Nintendo DS homebrew.

The original PC game is used as a behavioral reference for understanding and reproducing gameplay mechanics. This project does not attempt to run the original PC executable directly.

## Disclaimer

Balatro is the property of its respective developers and rights holders. This is an unofficial, independent fan project intended for educational and non-commercial purposes.

This repository does not intend to distribute the original game's proprietary assets or game files. Users must obtain the original game and its content through legitimate means.

No affiliation with or endorsement by the original developers or publishers is implied.

## Contributions

Suggestions, bug reports, and technical discussion are welcome. The project is currently experimental, and its implementation may differ from the original game in many areas.

## License

The licensing status of this project and its original code will be specified separately. Original Balatro assets, code, and other proprietary materials remain subject to their respective rights holders.

## support

If you’d like, you can help me—BUT PLEASE, support the creators of the original projects first (in this case, LocalThunk)

Without your support, many of these projects—whether from small studios or hobbyists—would not be possible. Thank you very much

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/B2Z629HLFH)

<p align="center">
  <a href="https://store.steampowered.com/app/2379780/Balatro/">
  <img src="https://lh5.googleusercontent.com/proxy/fi2zff--j67k1ZmoGJrtXi56Et1__2Mvz7ZSUsIgxIGhHeL9rX2YR-EbIFOMK07778D2gfWKZqnF0ZkrcJwkfOXBqIoXdw" alt="Buy now on Steam" width="300">
</a>
</p>
