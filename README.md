## Bilateral Filter for Torch

A Lua Torch wrapper for the fast bilateral filter by [Sylvain Paris and 
Frédo Durand](http://people.csail.mit.edu/sparis/bf/).

Can also be used for cross/joint bilateral filtering.


## Installation

Make sure you have [Torch](http://torch.ch/) installed.

To install use:
```bash
git clone https://github.com/dmarnerides/bilateral.git
cd bilateral
./install.sh
```

## Note:

* Only accepts two dimensional tensors, i.e. only luminance channels.
* Currently supports:
    * FloatTensor
    * DoubleTensor

## Use

### `filter(img,edge,sigma_s,sigma_r)`

Example:
```lua

local bilateral = require('bilateral')

-- Assuming a luminance channel lum, using default values
local filteredLum = bilateral.filter(lum)

-- Cross bilateral filter of lum given edgeLum, with given parameters
local filteredLum = bilateral.filter(lum,edgeLum,sigma_s,sigma_r)

```