import os
import sys
import time
import traceback as tb
from typing import List, Tuple, Dict, Set, Union, Callable, Optional

import numpy as np
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
from tqdm import tqdm 

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
import torchvision
import torchvision.models
from torchvision import transforms

from model import Resnet
from configuration import Configuration

def get_dataset():

    fashion_mnist_train = torchvision.datasets.FashionMNIST(
        root=Configuration.dataset_path,
        train=True,
        transform=torchvision.transforms.ToTensor(),
        download=True
    )
    print("number of images for training: ", len(fashion_mnist_train))

    fashion_mnist_test = torchvision.datasets.FashionMNIST(
        root=Configuration.dataset_path,
        train=False,
        transform=torchvision.transforms.ToTensor(),
        download=True
    )
    print("number of images for testing: ", len(fashion_mnist_test))

    train_dataloader = DataLoader(
        dataset=fashion_mnist_train,
        batch_size=Configuration.train_batch_size,
        shuffle=True,
        num_workers=4
    )

    test_dataloader = DataLoader(
        dataset=fashion_mnist_test,
        batch_size=Configuration.test_batch_size,
        shuffle=True,
        num_workers=4
    )

    return train_dataloader, test_dataloader


def train(device, num_epochs, train_dataloader, save_dir = None):
    
    model = Resnet(
        num_channels=Configuration.channels,
        num_out_features=Configuration.out_features,
        model_type='50'
    ).to(device)

    # print(model.model)

    optimizer = torch.optim.SGD(
        model.parameters(),
        lr=Configuration.learning_rate,
        momentum=Configuration.momentum
    )

    # loss_fn = nn.MSELoss(reduction='mean').to(device)
    loss_fn = nn.CrossEntropyLoss().to(device)

    num_iter = 0
    for epoch in range(num_epochs):
        # loop over mini-batches
        epoch_loss = 0
        running_loss = 0.0
        running_corrects = 0
        running_totals = 0
        for train_data in iter(train_dataloader):
            x_train = train_data[0].to(device)
            y_train = train_data[1].to(device)
            # 1. generate predictions
            pred = model(x_train) 
            # 2. calculate loss
            loss = loss_fn(pred, y_train)
            # 3. compute gradients
            loss.backward()
            # 4. update parameters using gradients
            optimizer.step()
            # 5. reset gradients to zero
            optimizer.zero_grad()
            # save training information
            running_loss += loss.item() * y_train.size(0)
            running_correct = (torch.argmax(pred, dim=1) == y_train).float().sum()
            running_corrects += running_correct
            running_totals += y_train.size(0)
            num_iter += 1
        # get training statistics for current epoch
        epoch_loss = running_loss / running_totals
        epoch_acc = running_corrects / running_totals
        print(f'Epoch: {epoch} | training Loss: {epoch_loss:.4f} | Acc: {epoch_acc:.4f}')

    # save model
    if save_dir:
        torch.save(model.state_dict(), save_dir)


def test(test_dataloader, load_dir, device='cpu'):

    model = Resnet(
        num_channels=Configuration.channels,
        num_out_features=Configuration.out_features,
        model_type='50'
    ).to(device)

    model.load_state_dict(torch.load(load_dir, map_location=torch.device(device)))
    model.eval()
    

    # check how system performs on the whole dataset
    correct = 0
    total = 0
    with torch.no_grad():
        for data in test_dataloader:
            images, labels = data
            images.to(device)
            labels.to(device)
            outputs = model.forward(images)
            total += labels.size(0)
            correct += (torch.argmax(outputs, dim=1) == labels).float().sum()
    
    print(f'Accuracy of the network on the 10000 test dataset: {100 * correct / total} %')


if __name__ == '__main__':

    device = 'cuda' if torch.cuda.is_available() else 'cpu'

    train_dataloader, test_dataloader = get_dataset()

    train(
        device=device,
        num_epochs=Configuration.num_epochs,
        train_dataloader=train_dataloader,
        save_dir=r'C:\Users\simon\Documents\Code\programming_experience\neural_networks\resnet\fashion_mnist.pth'
    )

    test(
        test_dataloader=test_dataloader,
        load_dir=r'C:\Users\simon\Documents\Code\programming_experience\neural_networks\resnet\fashion_mnist.pth'
    )