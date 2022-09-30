# frozen_string_literal: true

require 'bundler/setup'
require 'annoy'

if defined?(GC.verify_compaction_references) == 'method'
  # Calling this method will help to find object movement bugs.
  GC.verify_compaction_references(double_heap: true, toward: :empty)
end

RSpec.configure do |config|
  # Enable flags like --only-failures and --next-failure
  config.example_status_persistence_file_path = '.rspec_status'

  # Disable RSpec exposing methods globally on `Module` and `main`
  config.disable_monkey_patching!

  config.expect_with :rspec do |c|
    c.syntax = :expect
  end
end

module RSpec
  module Matchers
    module BuiltIn
      class BeWithin < BaseMatcher
        def matches?(actual)
          @actual = actual
          raise needs_expected unless defined? @expected

          if @expected.is_a?(Array) && @actual.is_a?(Array)
            @actual.zip(@expected).all? { |ac, ex| (ac - ex).abs <= @tolerance }
          else
            numeric? && (@actual - @expected).abs <= @tolerance
          end
        end
      end
    end
  end
end
