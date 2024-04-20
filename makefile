trex:
	@$(MAKE) -C $(src/trex)

clean:
	@$(MAKE) -C $(src/trex) clean

.PHONY: trex clean