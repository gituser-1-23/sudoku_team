#pragma once
// �ж�������Ƿ��ڰ�ť��
bool isButtonClicked(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight;
}

// �ж�����Ƿ���ͣ�ڰ�ť��
bool isButtonHovered(int x, int y, int buttonX, int buttonY, int buttonWidth, int buttonHeight)
{
    return isButtonClicked(x, y, buttonX, buttonY, buttonWidth, buttonHeight);
}