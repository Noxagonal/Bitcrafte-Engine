
# Vext engine test

cmake_minimum_required(VERSION 3.22)

get_auto_target_name(THIS_TARGET_NAME)

add_bitcrafte_test(${THIS_TARGET_NAME}
	PRIVATE_LIBRARY_DEPENDENCIES
		build_configuration
)
