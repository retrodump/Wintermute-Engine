class WmeExtension : public Extension
{
private:
	WmeExtension();
	WmeExtension(const WmeExtension &); // Disable copy ctor
	void operator=(const WmeExtension &);    // Disable operator=

public:
	static WmeExtension &Instance();

	// Implement the Extension interface
	virtual bool Initialise(ExtensionAPI *host_);
	virtual bool Finalise();
	virtual bool Clear();
	virtual bool Load(const char *filename);

private:
	ExtensionAPI* m_Host;
};
