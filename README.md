# Pixel Pattern Classifier

A lightweight, explainable 2D symbol recognition and pattern classification engine written from scratch in C++ without external machine learning libraries.

## Overview
The **Pixel Pattern Classifier** isolates binary symbol drawings from background noise, normalizes raw 2D pixel grids into standardized 16×16 (256-dimensional) feature vectors, and classifies symbols against a registered dictionary using vector similarity metrics and structural geometry.

## Key Features
* **Bounding Box Normalization:** Crops background whitespace from arbitrary input grids (up to 32×32) and maps them proportionally to a uniform 16×16 feature space.
* **Multi-Metric Scoring Algorithm:**
  * **Cosine Similarity (50% weight):** Measures vector orientation and pixel alignment in 256D space.
  * **Euclidean Distance (20% weight):** Quantifies point-to-point pixel separation.
  * **Structural Tiebreakers (30% weight):** Evaluates Horizontal Symmetry, Vertical Symmetry, Pixel Density, and Center of Mass.
* **Confidence Thresholding:** Employs a strict 70% confidence floor to reject heavily corrupted or random inputs rather than producing false positives.
* **Cached Templates:** Precomputes structural features upon template registry to eliminate redundant runtime calculations.

## Test Results
* **Clean Symbols (X, O, T):** 100% confidence classification.
* **Noisy Inputs:** 70–73% confidence classification with correct label.
* **Heavily Distorted / Random Inputs:** Correctly rejected (below 70% threshold).

## Authors
* **Justin Kim** ([@justin23236](https://github.com/justin23236))
* Hengmeng Ly
* Bella Ho

## Videos
* CS212 Pixel Pattern Classifier Final Presentation Video - https://www.youtube.com/watch?v=zTwagkgE3BE
* CS212 Pixel Pattern Classifier Final Project Demonstration - https://www.youtube.com/watch?v=PHIUPc31vpg

*CS 212 Final Project — Bellevue College (Instructor: Varik Hoang)*