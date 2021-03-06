PROJECT = langerl

DEPS = edown
dep_edown = git https://github.com/uwiger/edown.git master

CP = cp
CP_R = cp -r
RM_RF = rm -rf
DATE = $(shell date +"%F %T")

EDOC_OPTS = {doclet, edown_doclet} \
						, {app_default, "http://www.erlang.org/doc/man"} \
						, {source_path, ["src"]} \
						, {overview, "overview.edoc"} \
						, {stylesheet, ""} \
						, {image, ""} \
						, {edown_target, gitlab} \
						, {top_level_readme, {"./README.md", "https://gitlab.botsunit.com/msaas/langerl"}} 

include erlang.mk

app:: compile-nodes

clean::
	@rm -rf priv
	@rm -rf build

compile-nodes:
	@mkdir -p build
	cd build ; cmake .. ; make

dev: deps app
	@erl -name test -pa ebin include deps/*/ebin deps/*/include -config config/langerl.config

xtests: test-build app
	$(gen_verbose) $(ERL) -name tests -pa $(TEST_DIR) $(DEPS_DIR)/*/ebin ebin \
		-eval "$(subst $(newline),,$(subst ",\",$(call eunit.erl,$(EUNIT_MODS))))"

rel-dev: deps app
	@${RM_RF} ../langerl-dev
	git clone git@github.com:botsunit/langerl.git ../langerl-dev
	@${CP_R} ebin ../langerl-dev
	@${CP_R} config ../langerl-dev
	@${CP_R} include ../langerl-dev
	cd ../langerl-dev; git add . 
	cd ../langerl-dev; git commit -m "Update ${DATE}"

