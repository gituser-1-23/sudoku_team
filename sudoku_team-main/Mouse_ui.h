#pragma once
// 判断鼠标点击是否在按钮上
bool isButtonClicked(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

// 判断鼠标是否悬停在按钮上
bool isButtonHovered(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return isButtonClicked(x, y, buttonX, buttonY, buttonWidth, buttonHeight);
}