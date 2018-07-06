#include "NanoMeter.hpp"

START_NAMESPACE_DISTRHO

NanoMeter::NanoMeter(NanoWidget *widget, Size<uint> size) noexcept : NanoWidget(widget),
                                                                     fOutLeft(0),
                                                                     fOutRight(0)
{
    setSize(size);

    fColor = Color(82, 238, 248);
}

void NanoMeter::setOutLeft(float value) noexcept
{
    value = (fOutLeft * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

    if (value < 0.001f)
        value = 0.0f;
    else if (value > 0.999f)
        value = 1.0f;

    if (fOutLeft != value)
    {
        fOutLeft = value;
        repaint();
    }
}

void NanoMeter::setOutRight(float value) noexcept
{
    value = (fOutRight * kSmoothMultiplier + value) / (kSmoothMultiplier + 1.0f);

    if (value < 0.001f)
        value = 0.0f;
    else if (value > 0.999f)
        value = 1.0f;

    if (fOutRight != value)
    {
        fOutRight = value;
        repaint();
    }
}

void NanoMeter::onNanoDisplay()
{
    static const Color kColorBlack(0, 0, 0);
    static const Color kColorRed(255, 0, 0);
    static const Color kColorYellow(255, 255, 0);

    const float outLeft(fOutLeft);
    const float outRight(fOutRight);

    const float width = getWidth();
    const float height = getHeight();

    const float halfWidth = static_cast<float>(getWidth()) / 2.0f;
    const float redYellowHeight = static_cast<float>(getHeight()) * 0.2f;
    const float yellowBaseHeight = static_cast<float>(getHeight()) * 0.4f;
    const float baseBaseHeight = static_cast<float>(getHeight()) * 0.6f;

    // outline and socket
    beginPath();

    fillColor(Color(16, 16, 16, 255));
    strokeColor(Color(62, 71, 72, 255));

    const float socketStrokeWidth = 1.0f;

    strokeWidth(socketStrokeWidth);

    roundedRect(socketStrokeWidth, socketStrokeWidth, width - socketStrokeWidth * 2.0f, height - socketStrokeWidth * 2.0f, 0.5f);
    fill();
    stroke();

    closePath();

    // glass
    beginPath();

    const Color glassTopColor = Color(46, 46, 46, 255);
    const Color glassBottomColor = Color(30, 30, 30, 255);

    const Color glassTopOutlineColor = Color(74, 74, 74, 255);
    const Color glassBottomOutlineColor = Color(74, 74, 74, 120);

    fillPaint(linearGradient(halfWidth, 0, halfWidth, height, glassTopColor, glassBottomColor));
    strokePaint(linearGradient(halfWidth, 0, halfWidth, height, glassTopOutlineColor, glassBottomOutlineColor));

    const float glassStrokeWidth = 1.0f;
    const float glassRectXY = glassStrokeWidth + socketStrokeWidth + 0.5f;
    const float doubleGlassRectXY = glassRectXY * 2.f;

    strokeWidth(glassStrokeWidth);

    roundedRect(glassRectXY, glassRectXY, width - doubleGlassRectXY, height - doubleGlassRectXY, 0.5f);
    fill();
    stroke();

    closePath();

    // glass reflection
    beginPath();

    fillColor(94, 94, 101, 37);

    const float reflectionRightHeight = 9.f;

    moveTo(glassRectXY, glassRectXY);
    lineTo(width - glassRectXY, glassRectXY);
    lineTo(width - glassRectXY, glassRectXY + reflectionRightHeight);
    lineTo(glassRectXY, glassRectXY + reflectionRightHeight * 2.0f);
    lineTo(glassRectXY, glassRectXY);

    fill();

    closePath();

    // meters
    const float metersXY = glassRectXY + 2.0f;
    const float halfMetersXY = metersXY / 2.0f;
    const float leftRightMetersMargin = 2.0f;
    const float meterWidth = halfWidth - leftRightMetersMargin - glassRectXY * 2 + 2.0f;
    const float baseY = metersXY + redYellowHeight + yellowBaseHeight - 0.25f;
    const float baseHeight = height - metersXY - baseY;
    const float meterBottom = height - metersXY;

    // create gradients
    Paint fGradient1 = linearGradient(0.0f, 0.0f, 0.0f, redYellowHeight, kColorRed, kColorYellow);
    Paint fGradient2 = linearGradient(0.0f, redYellowHeight, 0.0f, yellowBaseHeight, kColorYellow, fColor);

    //left meter, then right one
    for (int i = 0; i < 2; ++i)
    {
        translate(i * (meterWidth + leftRightMetersMargin), 0);

        const float out = i == 0 ? outLeft : outRight;

        const float outputHeight = out * meterBottom;
        scissor(metersXY, meterBottom - outputHeight, meterWidth, outputHeight);

        beginPath();

        rect(metersXY, metersXY, meterWidth, redYellowHeight);
        fillPaint(fGradient1);
        fill();

        closePath();

        beginPath();

        rect(metersXY, metersXY + redYellowHeight - 0.25f, meterWidth, yellowBaseHeight);
        fillPaint(fGradient2);
        fill();

        closePath();

        beginPath();

        rect(metersXY, baseY, meterWidth, baseHeight);

        fillPaint(fGradient2);
        fill();

        closePath();
    }
}

END_NAMESPACE_DISTRHO