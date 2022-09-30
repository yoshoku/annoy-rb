# frozen_string_literal: true

RSpec.describe Annoy do
  it 'has a version number' do
    expect(Annoy::VERSION).not_to be_nil
  end

  describe Annoy::AnnoyIndex do
    let(:tol) { 1e-7 }
    let(:n_features) { 4 }
    let(:metric) { 'manhattan' }
    let(:dtype) { 'float64' }
    let(:n_trees) { 3 }
    let(:n_items) { 5 }
    let(:n_neighbors) { 3 }
    let(:index) { described_class.new(n_features: n_features, metric: metric, dtype: dtype) }

    before do
      index.seed(1)
      index.add_item(0, [0.0, 0.1, 0.2, 0.3])
      index.add_item(1, [0.1, 0.1, 0.2, 0.3])
      index.add_item(2, [0.1, 0.1, 0.2, 0.4])
      index.add_item(3, [0.2, 0.1, 0.2, 0.5])
      index.add_item(4, [0.2, 0.1, 0.2, 0.6])
      index.build(n_trees)
    end

    describe 'Index instance' do
      subject { index.instance_variable_get(:@index) }

      context "when metric is 'angular'" do
        let(:metric) { 'angular' }

        it { is_expected.to be_a(Annoy::AnnoyIndexAngular) }

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it { is_expected.to be_a(Annoy::AnnoyIndexAngularFloat32) }
        end
      end

      context "when metric is 'dot'" do
        let(:metric) { 'dot' }

        it { is_expected.to be_a(Annoy::AnnoyIndexDotProduct) }

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it { is_expected.to be_a(Annoy::AnnoyIndexDotProductFloat32) }
        end
      end

      context "when metric is 'hamming'" do
        let(:metric) { 'hamming' }

        it { is_expected.to be_a(Annoy::AnnoyIndexHamming) }
      end

      context "when metric is 'euclidean'" do
        let(:metric) { 'euclidean' }

        it { is_expected.to be_a(Annoy::AnnoyIndexEuclidean) }

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it { is_expected.to be_a(Annoy::AnnoyIndexEuclideanFloat32) }
        end
      end

      context "when metric is 'manhattan'" do
        let(:metric) { 'manhattan' }

        it { is_expected.to be_a(Annoy::AnnoyIndexManhattan) }

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it { is_expected.to be_a(Annoy::AnnoyIndexManhattanFloat32) }
        end
      end
    end

    describe '#get_nns_by_item' do
      context 'when include_distances is false' do
        subject { index.get_nns_by_item(0, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_item(0, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors', :aggregate_failures do
          expect(subject[0]).to match([0, 1, 2])
          expect(subject[1]).to be_within(tol).of([0.0, 0.1, 0.2])
        end

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it 'returns id list and distances of nearest neighbors', :aggregate_failures do
            expect(subject[0]).to match([0, 1, 2])
            expect(subject[1]).to be_within(tol).of([0.0, 0.1, 0.2])
          end
        end
      end
    end

    describe '#get_nns_by_vector' do
      let(:query) { [0.0, 0.1, 0.2, 0.3] }

      context 'when include_distances is false' do
        subject { index.get_nns_by_vector(query, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_vector(query, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors', :aggregate_failures do
          expect(subject[0]).to match([0, 1, 2])
          expect(subject[1]).to be_within(tol).of([0.0, 0.1, 0.2])
        end

        context 'with float32 data type' do
          let(:dtype) { 'float32' }

          it 'returns id list and distances of nearest neighbors', :aggregate_failures do
            expect(subject[0]).to match([0, 1, 2])
            expect(subject[1]).to be_within(tol).of([0.0, 0.1, 0.2])
          end
        end
      end
    end

    describe '#get_item' do
      subject { index.get_item(0) }

      it 'returns the item vector specified by ID' do
        expect(subject).to be_within(tol).of([0.0, 0.1, 0.2, 0.3])
      end

      context 'with float32 data type' do
        let(:dtype) { 'float32' }

        it 'returns the item vector specified by ID' do
          expect(subject).to be_within(tol).of([0.0, 0.1, 0.2, 0.3])
        end
      end
    end

    describe '#n_fetures' do
      subject { index.n_features }

      it 'returns the number of features' do
        expect(subject).to eq(n_features)
      end
    end

    describe '#metric' do
      subject { index.metric }

      it 'returns the distance metric' do
        expect(subject).to eq(metric)
      end
    end

    describe '#dtype' do
      subject { index.dtype }

      it 'returns the data type of feature' do
        expect(subject).to eq(dtype)
      end
    end

    describe '#n_items' do
      subject { index.n_items }

      it 'returns the number of indexed items' do
        expect(subject).to eq(n_items)
      end
    end

    describe '#n_trees' do
      subject { index.n_trees }

      it 'returns the number of index trees' do
        expect(subject).to eq(n_trees)
      end
    end

    describe '#save and #load' do
      context 'with an index created by annoy-rb' do
        let(:filename) { File.expand_path("#{__dir__}/test.ann") }

        before { index.save(filename) }

        it 'loads the saved index', :aggregate_failures do
          loaded = described_class.new(n_features: n_features, metric: metric)
          loaded.load(filename)
          expect(loaded.n_items).to eq(index.n_items)
          expect(loaded.n_trees).to eq(index.n_trees)
          expect(loaded.get_item(0)).to match(index.get_item(0))
        end
      end

      context 'with an index created by the Python bindings' do
        # from annoy import AnnoyIndex
        #
        # t = AnnoyIndex(4, 'angular')
        # t.add_item(0, [0.1, 0.2, 0.3, 0.4])
        # t.add_item(1, [0.5, 0.6, 0.7, 0.8])
        # t.add_item(2, [0.9, 0.0, 0.1, 0.2])
        # t.add_item(3, [0.3, 0.4, 0.5, 0.6])
        # t.add_item(4, [0.7, 0.8, 0.9, 0.0])
        # t.build(5)
        # t.save('pytest.ann')
        let(:filename) { File.expand_path("#{__dir__}/pytest.ann") }

        it 'loads the saved index', :aggregate_failures do
          loaded = described_class.new(n_features: 4, metric: 'angular', dtype: 'float32')
          loaded.load(filename)
          expect(loaded.n_items).to eq(5)
          expect(loaded.get_item(0)).to be_within(tol).of([0.1, 0.2, 0.3, 0.4])
          expect(loaded.get_item(1)).to be_within(tol).of([0.5, 0.6, 0.7, 0.8])
          expect(loaded.get_item(2)).to be_within(tol).of([0.9, 0.0, 0.1, 0.2])
          expect(loaded.get_item(3)).to be_within(tol).of([0.3, 0.4, 0.5, 0.6])
          expect(loaded.get_item(4)).to be_within(tol).of([0.7, 0.8, 0.9, 0.0])
        end
      end
    end

    describe '#get_distance' do
      subject { index.get_distance(0, 1) }

      before do
        index.unload
        index.add_item(0, [1, 0, 0, 1])
        index.add_item(1, [0, 1, 0, 1])
      end

      context "when metric is 'angular'" do
        let(:metric) { 'angular' }

        it 'returns cosine distance between items' do
          expect(subject).to eq(1)
        end
      end

      context "when metric is 'dot'" do
        let(:metric) { 'dot' }

        it 'returns dot product distance between items' do
          expect(subject).to eq(1)
        end
      end

      context "when metric is 'hamming'" do
        let(:metric) { 'hamming' }

        it 'returns hamming distance between items' do
          expect(subject).to eq(2)
        end
      end

      context "when metric is 'euclidean'" do
        let(:metric) { 'euclidean' }

        it 'returns euclidean distance between items' do
          expect(subject).to be_within(tol).of(1.41421356)
        end
      end

      context "when metric is 'manhattan'" do
        let(:metric) { 'manhattan' }

        it 'returns manhattan distance between items' do
          expect(subject).to eq(2)
        end
      end
    end
  end
end
