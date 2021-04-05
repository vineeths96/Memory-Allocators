![Language](https://img.shields.io/badge/language-C-blue) [![Contributors][contributors-shield]][contributors-url] [![Forks][forks-shield]][forks-url] [![Stargazers][stars-shield]][stars-url] [![Issues][issues-shield]][issues-url] [![MIT License][license-shield]][license-url] [![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />

<p align="center">
  <h3 align="center">Memory Allocators</h3>
  <p align="center">
    OneBin Allocator and SPHeap Allocator  
    <br />
    <a href=https://github.com/vineeths96/Memory-Allocators><strong>Explore the repository»</strong></a>
    <br />
    <br />
    <a href=https://github.com/vineeths96/Memory-Allocators/blob/master/Problem%20Statement.pdf>View Problem Statement</a>
    </p>






</p>

> tags : memory allocator, weighted buddy, buddy, onebin, exponential buddy system



<!-- ABOUT THE PROJECT -->

## About The Project

This project deals with the implementation of `Weighted Buddy Method for Dynamic Storage Allocation` proposed by Shen & Peterson and `OneBin Dynamic Storage Allocation`. We keep track of the memory space, Available Space List, KVAL, TAG and TYPE fields for the memory blocks centrally. We also keep track of the numbers of allocation requests, deallocation requests, area splits, buddy recombinations, internal fragmentation and external fragmentation. The program has been segregated to two folders:

- Tests: Tests folder contain the SPHeap implementation with the uniform and exponential synthetic testing for performance evaluation.
- Polynomial: Polynomial folder contains the implementing of SPHeap and OneBin memory allocation in the polynomial arithmetic program.

Within each directory, there are sub-directories corresponding to the memory allocators implemented. The program for each data structure is written in three files - `client.c`, `header.c`, and `implement.c`. Comments have been added frequently to help in understanding the logic behind implementation. Refer [Problem statement](./Problem%20Statement.pdf) file for detailed information.



### Built With
This project was built with 

* C
* Ubuntu 18.04.1 
* gcc version 7.4.0



<!-- GETTING STARTED -->

## Getting Started

Clone the repository into a local machine using

```shell
git clone https://github.com/vineeths96/Memory-Allocators
```

### Instructions to run

##### Synthetic Testing

Open the terminal, and run the following command where `TestMethod` is `Exponential` or `Uniform`.   

```
cd Tests/<TestMethod>
```

Make the program and run it.

```shell
make
./a.out
```

##### Practical Testing

Open the terminal, and run the following command where `AllocMethod` is `OneBin` or `SPHeap`.  

```
cd Polynomial/<AllocMethod>
```

Make the program and run it.

```shell
make
./a.out
```



<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Vineeth S - vs96codes@gmail.com

Project Link: [https://github.com/vineeths96/Memory-Allocators](https://github.com/vineeths96/Memory-Allocators)




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/vineeths96/Memory-Allocators.svg?style=flat-square
[contributors-url]: https://github.com/vineeths96/Memory-Allocators/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/vineeths96/Memory-Allocators.svg?style=flat-square
[forks-url]: https://github.com/vineeths96/Memory-Allocators/network/members
[stars-shield]: https://img.shields.io/github/stars/vineeths96/Memory-Allocators.svg?style=flat-square
[stars-url]: https://github.com/vineeths96/Memory-Allocators/stargazers
[issues-shield]: https://img.shields.io/github/issues/vineeths96/Memory-Allocators.svg?style=flat-square
[issues-url]: https://github.com/vineeths96/Memory-Allocators/issues
[license-shield]: https://img.shields.io/badge/License-MIT-yellow.svg
[license-url]: https://github.com/vineeths96/Memory-Allocators/blob/master/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/vineeths

