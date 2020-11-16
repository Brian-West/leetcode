import torch
import torch.nn as nn

class BottleNeck(nn.Module):
    expansion = 4

    def __init__(self, in_channels, out_channels, stride):
        super().__init__()
        
        self.residual_function = nn.Sequential(
            nn.Conv2d(in_channels, out_channels, 1, 1, 0, bias=False),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channels, out_channels, 3, stride, 1, bias=False),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channels, out_channels*BottleNeck.expansion, 1, 1, 0, bias=False),
            nn.BatchNorm2d(out_channels*BottleNeck.expansion)
        )

        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels*BottleNeck.expansion:
            self.shortcut = nn.Sequential(
                nn.Conv2d(in_channels, out_channels*BottleNeck.expansion, 1, stride, 0, bias=False),
                nn.BatchNorm2d(out_channels*BottleNeck.expansion)
            )
    
    def forward(self, x):
        return nn.ReLU(inplace=True)(self.residual_function(x) + self.shortcut(x))

class BasicBlock(nn.Module):
    # 每一个block的输出通道数是输入通道数的多少倍
    expansion = 1

    def __init__(self, in_channels, out_channels, stride):
        super().__init__()

        self.residual_function = nn.Sequential(
            nn.Conv2d(in_channels, out_channels, 3, stride, 1, bias=False),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channels, out_channels*BasicBlock.expansion, 3, 1, 1, bias=False),
            nn.BatchNorm2d(out_channels*BasicBlock.expansion)
        )

        self.shortcut = nn.Sequential()
        if stride != 1 or in_channels != out_channels*BasicBlock.expansion:
            self.shortcut = nn.Sequential(
                nn.Conv2d(in_channels, out_channels*BasicBlock.expansion, 1, stride, 0, bias=False),
                nn.BatchNorm2d(out_channels*BasicBlock.expansion)
            )

    def forward(self, x):
        return nn.ReLU(inplace=True)(self.residual_function(x) + self.shortcut(x))

class DecodeBlock(nn.Module):
    expansion = 1

    def __init__(self, in_channels, out_channels, stride):
        super().__init__()
        self.head = nn.Conv2d(in_channels, out_channels, kernel_size=3, stride=stride, padding=1, bias=False)
        if stride!=1:
            self.head = nn.ConvTranspose2d(in_channels, out_channels, kernel_size=3, stride=stride, padding=1, output_padding=1, bias=False)

        self.decode_residual = nn.Sequential(
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True),
            nn.Conv2d(out_channels, out_channels*DecodeBlock.expansion, 3, 1, 1, bias=False),
            nn.BatchNorm2d(out_channels*DecodeBlock.expansion)
        )
        self.shortcut = nn.Sequential()
        if stride!=1 or in_channels!=out_channels*DecodeBlock.expansion:
            self.shortcut = nn.Sequential(
                nn.ConvTranspose2d(in_channels, out_channels*DecodeBlock.expansion, kernel_size=3, stride=stride, padding=1, output_padding=1, bias=False),
                nn.BatchNorm2d(out_channels*DecodeBlock.expansion)
            )

    def forward(self, x):
        return nn.ReLU(inplace=True)(self.decode_residual(self.head(x)) + self.shortcut(x))

def gen_upsampling(in_channels, out_channels, stride=2):
    upsampling = nn.Sequential(
        nn.ConvTranspose2d(in_channels, out_channels, kernel_size=3, stride=stride, padding=1, output_padding=1, bias=False),
        nn.BatchNorm2d(out_channels),
        nn.ReLU(inplace=True)
    )
    return upsampling

class ResUNet(nn.Module):
    def __init__(self, block, num_blocks, input_shape):
        super().__init__()

        self.in_channels = 64

        self.enco_conv1 = self._make_layer(block, 1, num_blocks[0], 64)
        self.enco_conv2 = self._make_layer(block, 2, num_blocks[1], 128)
        self.enco_conv3 = self._make_layer(block, 2, num_blocks[2], 256)
        self.enco_conv4 = self._make_layer(block, 2, num_blocks[3], 512)

        self.upsampling1 = gen_upsampling(self.in_channels, 256*block.expansion)
        self.in_channels = 256*block.expansion
        # self.deco_conv4 = self._make_layer(DecodeBlock, 2, num_blocks[3], 256)
        self.deco_conv3 = self._make_layer(block, 1, num_blocks[2], 256)
        self.upsampling2 = gen_upsampling(self.in_channels, 128*block.expansion)
        self.in_channels = 128*block.expansion
        self.deco_conv2 = self._make_layer(block, 1, num_blocks[1], 128)
        self.upsampling3 = gen_upsampling(self.in_channels, 64*block.expansion)
        self.in_channels = 64*block.expansion
        self.deco_conv1 = self._make_layer(block, 1, num_blocks[0], 64)

    def _make_layer(self, block, stride, num_blocks, out_channels):
        strides = [stride] + [1]*(num_blocks-1)
        layer = []
        for stride in strides:
            layer.append(block(self.in_channels, out_channels, stride))
            self.in_channels = out_channels*block.expansion
        return nn.Sequential(*layer)

    def forward(self, x):
        conv1_output = self.enco_conv1(x)
        conv2_output = self.enco_conv2(conv1_output)
        conv3_output = self.enco_conv3(conv2_output)
        conv4_output = self.enco_conv4(conv3_output)

        conv4_output = self.upsampling1(conv4_output)
        deconv3_input = torch.add(conv4_output, conv3_output)
        deconv3_output = self.deco_conv3(deconv3_input)

        deconv3_output = self.upsampling2(deconv3_output)
        deconv2_input = torch.add(deconv3_output, conv2_output)
        deconv2_output = self.deco_conv2(deconv2_input)

        deconv2_output = self.upsampling3(deconv2_output)
        deconv1_input = torch.add(deconv2_output, conv1_output)
        deconv1_output = self.deco_conv1(deconv1_input)

        # deconv4_output = self.deco_conv4(conv4_output)

        # deconv3_input = torch.add(deconv4_output, conv3_output)
        # deconv3_output = self.deco_conv3(deconv3_input)

        # deconv2_input = torch.add(deconv3_output, conv2_output)
        # deconv2_output = self.deco_conv2(deconv2_input)

        # deconv1_input = torch.add(deconv2_output, conv1_output)
        # deconv1_output = self.deco_conv1(deconv1_input)
        
        return deconv1_output