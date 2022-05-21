import os
import sys
import time
import traceback as tb
from typing import List, Sequence, Tuple, Dict, Set, Union, Callable, Optional

import numpy as np
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.models
from torchvision import transforms

class Block(nn.Module):
    def __init__(
        self, 
        in_channels: int, 
        out_channels: int, 
        stride: int = 1,
        identity_downsample = None
    ):
        super().__init__()
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.stride = stride
        self.identity_downsample = identity_downsample

        self.model = nn.Sequential(
            # (1.)
            nn.BatchNorm2d(
                num_features=self.in_channels, 
                track_running_stats=False
            ),
            nn.ReLU(inplace=True),
            nn.Conv2d(
                in_channels=self.in_channels, 
                out_channels=self.out_channels, 
                kernel_size=1,
                stride=self.stride, #1,
                padding=0,
                bias=False
            ),
            # (2.)
            nn.BatchNorm2d(
                num_features=self.out_channels,
                track_running_stats=False
            ),
            nn.ReLU(inplace=True),
            nn.Conv2d(
                in_channels=self.out_channels, 
                out_channels=self.out_channels, 
                kernel_size=3,
                stride=1, #self.stride,
                padding=1,
                bias=False
            ),
            # (3.)
            nn.BatchNorm2d(
                num_features=self.out_channels,
                track_running_stats=False
            ),
            nn.ReLU(inplace=True),
            nn.Conv2d(
                in_channels=self.out_channels, 
                out_channels=self.out_channels*4, 
                kernel_size=1,
                stride=1,
                padding=0,
                bias=False
            )   
        )

    def forward(self, input):
        output = self.model(input)
        if self.identity_downsample:
            input = self.identity_downsample(input)
        output += input
        return output

class Resnet(nn.Module):
    def __init__(
        self,
        num_channels: int = 1, 
        num_out_features: int = 10,
        model_type = "152"
    ):
        super().__init__()
        
        strides_layers = [1, 2, 2, 2]

        if model_type in ('50', '101', '152'):
            out_channels_layers = (64, 128, 256, 512)

        if model_type == "50":
            num_blocks_per_layer = (3, 4, 6, 3)

        elif model_type == "101":
            num_blocks_per_layer = (3, 4, 23, 3)

        elif model_type == "152":
            num_blocks_per_layer = (3, 8, 36, 3)
            
        else:
            raise NotImplementedError("Available Models: '50', '101', '152'")

        self.in_channels = 64
        self.num_channels = num_channels
        self.num_out_features = num_out_features
        self.num_blocks_per_layer = num_blocks_per_layer

        self.model = nn.Sequential(
            nn.Conv2d(
                in_channels=self.num_channels,
                out_channels=64,
                kernel_size=7,
                stride=2,
                padding=3,
                bias=False  
            ),
            nn.BatchNorm2d(
                num_features=64,
                track_running_stats=False
            ),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(
                kernel_size=3,
                stride=2,
                padding=1
            ),
            *[self.get_block_layer(
                num_blocks=self.num_blocks_per_layer[i],
                out_channels=out_channels_layers[i],
                stride=strides_layers[i]
            ) for i in range(4)],
            nn.AdaptiveAvgPool2d(output_size=(1, 1)),
            nn.Flatten(),
            nn.Linear(
                in_features=512*4,
                out_features=self.num_out_features,
                bias=True
            )
        )

    def get_block_layer(self, num_blocks: int, out_channels: int, stride: int):

        identity_downsample = None
        if (stride != 1) or (self.in_channels != 4 * out_channels):
            identity_downsample = nn.Sequential(
                nn.Conv2d(
                    in_channels=self.in_channels, 
                    out_channels=out_channels * 4,
                    kernel_size=1,
                    stride=stride,
                ),
                nn.BatchNorm2d(num_features=out_channels * 4)
            )

        layers = nn.Sequential(
            Block(
                in_channels=self.in_channels, 
                out_channels=out_channels, 
                stride=stride,
                identity_downsample=identity_downsample
            )
        )

        self.in_channels = out_channels * 4

        for _ in range(1, num_blocks):
            layers.append(
                Block(
                    in_channels=self.in_channels,
                    out_channels=out_channels,
                    stride=1,
                    identity_downsample=None
                )
            )

        return layers

    def forward(self, input):
        output = self.model(input)
        return output

if __name__ == '__main__':

    device = 'cuda' if torch.cuda.is_available() else 'cpu'

    model = Resnet(
        num_channels=1,
        num_out_features=10,
        model_type='152'
    ).to(device)

    x = torch.rand(64, 1, 28, 28).to(device)

    out = model(x)
    print(out.shape)


    #print(model.model) 
    # print(model._modules)

    