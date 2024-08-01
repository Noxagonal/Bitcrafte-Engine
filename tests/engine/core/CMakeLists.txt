
# Bitcrafte engine test

cmake_minimum_required(VERSION 3.22)

get_auto_test_target_name(THIS_TARGET_NAME)



# Add the test.
add_bitcrafte_test(${THIS_TARGET_NAME}
	PRIVATE_LIBRARY_DEPENDENCIES
		build_configuration
		core
)
