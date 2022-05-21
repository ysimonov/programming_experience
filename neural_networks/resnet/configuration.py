class Configuration:

    # model configuration
    channels = 1    # number of RGB(A) channels
    out_features = 10   # number of classified labels

    # training configuration
    learning_rate = 0.005
    momentum = 0.9

    num_epochs = 15
    train_batch_size = 2048 #512    # minibatch size
    test_batch_size = 80

    dataset_path = r"C:\Users\simon\Documents\Code\programming_experience\neural_networks\data"