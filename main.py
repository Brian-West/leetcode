from resunet import BasicBlock, ResUNet, BottleNeck
from torchsummary import summary

def resunet18(shape):
    return ResUNet(BasicBlock, [2, 2, 2, 2], shape)

def resunet34(shape):
    return ResUNet(BasicBlock, [3, 4, 6, 3], shape)

def resunet50(shape):
    return ResUNet(BottleNeck, [3, 4, 6, 3], shape)

if __name__ == '__main__':
    resunet = resunet50((256, 256))
    summary(resunet, (64, 256, 256))