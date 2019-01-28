#pragma once

class IBasis
{
public:
	IBasis() = default;
	virtual ~IBasis() = default;
public :
	virtual bool Init() noexcept = 0;
	virtual bool Frame() noexcept = 0;
	virtual bool Render() noexcept = 0;
	virtual bool Release() noexcept = 0;
};

